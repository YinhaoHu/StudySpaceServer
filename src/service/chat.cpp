#include"chat.hpp"
#include"notice.hpp"
#include"../server/net.hpp"
#include"../../include/extern_onlineuser.hpp"

#include<unordered_map>
#include<queue>
#include<string>
#include<mutex>
#include<memory>

using namespace std;

unordered_map<userID_t, queue<shared_ptr<wstring>>>* 
        privateMsgQueues = new unordered_map<userID_t, queue<shared_ptr<wstring>>>;

unordered_map<userID_t, queue<shared_ptr<wstring>>>* 
        privateMsg_IDQueues = new unordered_map<userID_t, queue<shared_ptr<wstring>>>;
mutex *privateMsgQueues_lock = new mutex;

//forward msg
void do_privateChatSend(serviceInfo* info)
{
    auto receiverID = easyRecv_WString(info->userfd);
    auto msg = easyRecv_WString(info->userfd);

    auto receiverIDNum = stoul(*receiverID);

    {
        unique_lock lock(*privateMsgQueues_lock);
        auto& msg_queue = *privateMsgQueues;
        auto& id_queue = *privateMsg_IDQueues;
        
        id_queue[receiverIDNum].push(make_shared<wstring>(info->userid_str));
        msg_queue[receiverIDNum].push(msg);
    }
    noticeUser(receiverIDNum, make_shared<wstring>(L"PRIVATE_CHAT_RECV"));
}

//send to the Client which requrie msg.
void do_privateChatRecv(serviceInfo* info)
{
    {
        unique_lock lock(*privateMsgQueues_lock);
        auto& msg_queue = (*privateMsgQueues)[info->userid];
        auto& id_queue = (*privateMsg_IDQueues)[info->userid];

        while(!id_queue.empty())
        {
            auto senderID = id_queue.front();
            auto msg = msg_queue.front();

            easySendMsg(info->userfd, make_shared<wstring>(L"PRIVATE_CHAT_RECV"));
            easySend_WString(info->userfd, senderID);
            easySend_WString(info->userfd, msg);
            id_queue.pop();
            msg_queue.pop();
        }
    }
}