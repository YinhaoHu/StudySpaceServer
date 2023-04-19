//service.cpp: responde the requested service from the client.
/*

----------------------------------------------------------------------------------------------------
                                    + SERVICE LIST +

    INDEX       SERVICE_NAME            INFO
    1           REGISTER                X
    2           LOGIN                   X
    3           COMCHATSEND             user send a msg in comchat service
    4           USERQUIT                get the quit signal from user and delete it from onlineUSers
    5           AUTOLOGIN               Permit the client to login directly.
----------------------------------------------------------------------------------------------------

*/
#include"service.hpp"
#include"register.hpp"
#include"login.hpp"
#include"comchatsend.hpp"
#include"autologin.hpp"
#include"userinfo_change.hpp"
#include"sendfriendslist.hpp"
#include"friend.hpp"
#include"notice.hpp"

#include"../server/net.hpp"
#include"../../include/extern_onlineuser.hpp"

#include<queue>
#include<functional>
#include<unordered_map>
#include<mutex>

#include<cstring>
using namespace std;


static bool inline matchProg(const wchar_t* prog, serviceInfo* info);

void servicePerform(serviceInfo* info)
{
    if(matchProg(L"REGISTER", info) )
    {
        doRegister(info);
    }
    else if(matchProg(L"LOGIN", info))
    {
        doLogin(info);
    }
    else if(matchProg(L"COMCHATSEND", info))
    {
        doComChatSend(info);
    }
    else if(matchProg(L"USERQUIT", info))
    {
        onlineUsersMutexLock->lock();
        onlineUsers->erase(info->userid);
        onlineUsersMutexLock->unlock();
    }
    else if(matchProg(L"AUTOLOGIN", info))
    {
        doAutoLogin(info);
    }
    else if(matchProg(L"USERINFO_CHANGE_GET_OLDPASSWORD",info))
    {
        do_userinfo_change_getOldPassword(info);
    }
    else if(matchProg(L"CHANGEUSERINFO", info))
    {
        do_changeUserInfo(info);
    }
    else if(matchProg(L"GETFRIENDSLIST",info))
    {
        doSendFriendsList(info);
    }
    else if(matchProg(L"ADDFRIEND_REQUEST",info))
    {
        doAddFriendRequest(info);
    }
    else if(matchProg(L"ADDFRIEND_CONFIRM",info))
    {
        doAddFriendResponse(info);
    }
    else if(matchProg(L"GET_NEW_FRIEND_INFO", info))
    {
        doGetNewFriendInfo(info);
    }
    else if(matchProg(L"GET_NOTIFICATION", info))
    {
        doGetNotification(info);
    }
    else if(matchProg(L"REQUEST_REMOVE_FRIEND", info))
    {
        doRemovoeFriendRequest(info);
    }
    else
    {  
        wprintf(L"UNKNOW PROG\n");
    }
}

static bool inline matchProg(const wchar_t* prog, serviceInfo* info)
{
    bool res = ( wcscmp(prog, info->prog) == 0);

    if(res)
        showMinior(L"Start Service", prog);

    return res;
}