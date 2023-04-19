#include"notice.hpp"

#include"../../include/extern_onlineuser.hpp"
#include"../server/net.hpp"

using namespace std;

void noticeUser(userID_t id,std::shared_ptr<std::wstring> msg)
{
    /*
                        Notification Format
        +   NOTICE
        +   MSG(also can be used as args with splited character space) 
    */
    bool isOnline;
    {
        unique_lock lock(*onlineUsersMutexLock);
        isOnline = onlineUsers->find(id) != onlineUsers->end();
    }

    if(isOnline)//online
    {
        {
            unique_lock lock(*onlineUsersMutexLock);
            easySendMsg(onlineUsers->at(id), make_shared<wstring>(L"NOTICE"));
            easySend_WString(onlineUsers->at(id), msg);
            showMinior(L"Finish", L"Noticed  user one message.");
        }
    }
    else//off line
    {
        {
            unique_lock lock(*userOfflineWaitingTasks_MutexLock);
            auto& userTasks = (*userOfflineWaitingTasks)[id];
            userTasks.push(msg);
        }
    }
}

void doGetNotification(serviceInfo* info)
{
    auto id = info->userid;
        {
        unique_lock lock(*userOfflineWaitingTasks_MutexLock);

        if(userOfflineWaitingTasks->contains(id))
        {    
            auto tasks = userOfflineWaitingTasks->at(id);
            while(!tasks.empty())
            {
                auto task = tasks.front();
                tasks.pop();
                noticeUser(id, task);
            }
        }
    }
}