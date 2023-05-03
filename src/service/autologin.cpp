#include"autologin.hpp"

#include"../../lib/HData/HData.hpp"
#include"../../include/extern_onlineuser.hpp"
#include"../server/net.hpp"

#include<unordered_map>
#include<mutex>

using namespace ceh::Data;
using namespace std;

extern void sendUserInit(userID_t id, int sockfd);//From login.hpp
extern void broadcast_online_friend(serviceInfo* info);//from login.cpp

void doAutoLogin(serviceInfo* info)
{
    {
        unique_lock lock(*onlineUsersMutexLock);

        if(onlineUsers->contains(info->userid))
        {    
            easySend_WString(info->userfd, make_shared<wstring>(L"AUTOLOGIN_ISONLINE"));
            return;
        }
        else
        {    
            easySend_WString(info->userfd, make_shared<wstring>(L"AUTOLOGIN_OK"));
            onlineUsers->emplace(info->userid, info->userfd);
        }
    }
    
    sendUserInit(info->userid, info->userfd);
    broadcast_online_friend(info);
}