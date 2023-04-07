//service.cpp: responde the requested service from the client.
/*

----------------------------------------------------------------------------------------------------
                                    + SERVICE LIST +

    INDEX       SERVICE_NAME            INFO
    1           REGISTER                X
    2           LOGIN                   X
    3           COMCHATSEND             user send a msg in comchat service
    4           USERQUIT                get the quit signal from user and delete it from onlineUSers
    
----------------------------------------------------------------------------------------------------

*/
#include"service.hpp"
#include"register.hpp"
#include"login.hpp"
#include"comchatsend.hpp"

#include<unordered_map>
#include<mutex>

#include<cstring>
using namespace std;

unordered_map<userID_t, userFD_t> onlineUsers;
mutex onlineUsersMutexLock;

void servicePerform(serviceInfo* info)
{
    if(wcscmp(L"REGISTER", info->prog) == 0)
    {
        doRegister(info);
    }
    else if(wcscmp(L"LOGIN", info->prog) == 0)
    {
        doLogin(info);
    }
    else if(wcscmp(L"COMCHATSEND", info->prog) == 0)
    {
        doComChatSend(info);
    }
    else if(wcscmp(L"USERQUIT", info->prog) == 0)
    {
        onlineUsersMutexLock.lock();
        onlineUsers.erase(info->userid);
        onlineUsersMutexLock.unlock();
    }
    else
        wprintf(L"UNKNOW PROG\n");
}