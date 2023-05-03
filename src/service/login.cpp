#include"login.hpp"
#include"notice.hpp"
#include"userdata.hpp"
#include"../server/net.hpp"
#include"../server/guard.hpp"
#include"../server/standard.hpp"
#include"../../lib/HData/HData.hpp"
#include"../../lib/HString/HString.hpp"
#include"../../include/extern_onlineuser.hpp"
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<unistd.h>

#include<fstream>
#include<unordered_map>
#include<mutex>

#include<cstdio>
#include<cstring>
#include<cwchar>

using namespace std;
using namespace ceh::String;

static auto& usersFile = standard::userDataFile;

static inline int checkInfo(wchar_t* id_str, userID_t id,wchar_t* password);

void broadcast_online_friend(serviceInfo* info);
void sendUserInit(userID_t id, int sock);

void doLogin(serviceInfo* info)
{
    wchar_t password[maxFieldSize], id[maxFieldSize], sendBUf[maxSockBufferSize];
    char sendMsg[maxSockBufferBytes];
    int statusCode;

    uintToWStr(info->userid, id);
    wcscpy(password, info->args);
    statusCode = checkInfo(id, info->userid,password);
    switch (statusCode)
    {
        case 1: 
            wcscpy(sendBUf, L"OK");
            onlineUsersMutexLock->lock();
           (* onlineUsers)[info->userid] = info->userfd;
            onlineUsersMutexLock->unlock();
            break;
        case 2: wcscpy(sendBUf, L"WRONG");break;
        case 3: wcscpy(sendBUf,L"NOTEXIST");break;
        case 4: wcscpy(sendBUf, L"LOGINED");break;
        default: wcscpy(sendBUf, L"UNKNOWN");
    }
    memcpy(sendMsg, sendBUf,maxSockBufferBytes);
    send(info->userfd, sendMsg, maxSockBufferBytes, 0);
    guard::monitor(L"SEND",sendBUf);

    if(statusCode == 1)
        sendUserInit(info->userid, info->userfd);

}

static inline int checkInfo(wchar_t* id_str, userID_t id,wchar_t* password)
{
    int statusCode;
  
    statusCode = 1;
    if( data::user::exist_id(id) == false)
        statusCode = 3;
    else if(data::user::get_userPassword(id)->compare(password) != 0)
        statusCode = 2;
    else if(onlineUsers->contains(id))
        statusCode = 4;
    return statusCode;
}

void broadcast_online_friend(serviceInfo* info)
{
    {
        unique_lock lock(*onlineUsersMutexLock);
        auto friends = data::user::get_userFriends(info->userid);
        for(auto fr : *friends)
        {
            auto fid = stoul(fr);
            if(onlineUsers->contains(fid))
            {
                auto ffd = onlineUsers->at(fid);
                easySendMsg(ffd, make_shared<wstring>(L"FRIEND_ONLINE"));
                easySend_WString(ffd, make_shared<wstring>(info->userid_str));
            }
        }
    }
}

void sendUserInit(userID_t id,int sockfd){
    auto send0_id = data::user::get_userID(id);
    auto send1_username = data::user::get_userName(id);
    auto send2_userintro = data::user::get_userIntro(id);
    auto send3_profile = data::user::get_userProfile(id);

    easySend_WString(sockfd, send0_id);
    easySend_WString(sockfd, send1_username);
    easySend_WString(sockfd, send2_userintro);
    easySend_File(sockfd, send3_profile->data, send3_profile->bytes);

    guard::monitor(L"FINISH ", L"SEND USERINIT()");
}