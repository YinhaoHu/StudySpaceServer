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
#include"chat.hpp"
#include"feedback.hpp"
#include"userquit.hpp"

#include"../server/guard.hpp"
#include"../server/net.hpp"
#include"../../include/extern_onlineuser.hpp"

#include<queue>
#include<vector>
#include<utility>
#include<functional>
#include<unordered_map>
#include<mutex>

#include<cstring>

using namespace std;
using namespace guard;

vector<pair<wstring, function<void(serviceInfo* info)>>> serviceProgs{
    {L"REGISTER", doRegister},
    {L"LOGIN", doLogin},
    {L"COMCHATSEND" , doComChatSend},
    {L"USERQUIT" , doUserQuit},
    {L"AUTOLOGIN" , doAutoLogin},
    {L"USERINFO_CHANGE_GET_OLDPASSWORD" , do_userinfo_change_getOldPassword},
    {L"CHANGEUSERINFO" , do_changeUserInfo},
    {L"GETFRIENDSLIST" , doSendFriendsList},
    {L"ADDFRIEND_REQUEST", doAddFriendRequest},
    {L"ADDFRIEND_CONFIRM" , doAddFriendResponse},
    {L"GET_NEW_FRIEND_INFO" ,doGetNewFriendInfo },
    {L"GET_NOTIFICATION", doGetNotification},
    {L"REQUEST_REMOVE_FRIEND" , doRemovoeFriendRequest},
    {L"PRIVATE_CHAT_SEND" , do_privateChatSend},
    {L"PRIVATE_CHAT_RECV" , do_privateChatRecv},
    {L"FEEDBACK" , doGetFeedBack}
};


void servicePerform(serviceInfo* info)
{
    for(const auto& prog : serviceProgs)
        if(prog.first.compare(info->prog) == 0)
        {    
            monitor(L"Start Service", info->prog);
            prog.second(info);
        }

    monitor(L"Finish Service", info->prog);
}