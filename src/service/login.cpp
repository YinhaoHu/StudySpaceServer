#include"login.hpp"
#include"notice.hpp"
#include"userdata.hpp"
#include"../server/net.hpp"
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
using namespace ceh::Data;
using namespace ceh::String;

const char* usersFile = "data/users.hdat";

static inline int checkInfo(wchar_t* id_str, userID_t id,wchar_t* password, HWData* data);
void sendUserInit(userID_t id, int sock);


void doLogin(serviceInfo* info)
{
    wchar_t password[maxFieldSize], id[maxFieldSize], sendBUf[maxSockBufferSize];
    char sendMsg[maxSockBufferBytes];
    HWData userdata(usersFile);
    int statusCode;

    userdata.load();
    uintToWStr(info->userid, id);
    wcscpy(password, info->args);
    statusCode = checkInfo(id, info->userid,password, &userdata);
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
    showMinior(L"SEND",sendBUf);

    if(statusCode == 1)
        sendUserInit(info->userid, info->userfd);

    userdata.save();
}

static inline int checkInfo(wchar_t* id_str, userID_t id,wchar_t* password, HWData* data)
{
    int statusCode;
  
    statusCode = 1;
    if(-1 == data->findKey(id_str))
        statusCode = 3;
    else if(wcscmp(data->access(id-standard::userid_begin).values[standard::userDataField::password].c_str(), password))
        statusCode = 2;
    else if(onlineUsers->find(id) != onlineUsers->end())
        statusCode = 4;
    return statusCode;
}

void sendUserInit(userID_t id,int sockfd){
    auto send0_id = serverData::get_userID(id);
    auto send1_username = serverData::get_userName(id);
    auto send2_userintro = serverData::get_userIntro(id);
    auto send3_profile = serverData::get_userProfile(id);

    easySend_WString(sockfd, send0_id);
    easySend_WString(sockfd, send1_username);
    easySend_WString(sockfd, send2_userintro);
    easySend_File(sockfd, send3_profile->data, send3_profile->bytes);

    showMinior(L"FINISH ", L"SEND USERINIT()");
}