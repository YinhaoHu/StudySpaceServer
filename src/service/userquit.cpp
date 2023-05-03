#include"userquit.hpp"
#include"../../include/extern_onlineuser.hpp"
#include"userdata.hpp"
#include"../server/net.hpp"


using namespace data::user;
using namespace std;

static inline void broadcast_offline_friend(serviceInfo* info);

void doUserQuit(serviceInfo* info)
{
    {
        unique_lock lock(*onlineUsersMutexLock);
        onlineUsers->erase(info->userid);
    }
    broadcast_offline_friend(info);
}

static inline void broadcast_offline_friend(serviceInfo *info)
{
    auto friends = get_userFriends(info->userid);
    {
        unique_lock lock(*onlineUsersMutexLock);
        for(auto& fri : *friends)
        {
            auto fid = stoul(fri);
            if(onlineUsers->contains(fid))
            {
                auto ffd = (*onlineUsers)[fid];
                easySendMsg(ffd, make_shared<wstring>(L"FRIEND_OFFLINE"));
                easySend_WString(ffd, make_shared<wstring>(info->userid_str));
            }
        }
    }
}