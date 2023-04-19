#include"sendfriendslist.hpp"
#include"userdata.hpp"
#include"../server/net.hpp"
#include"../../lib/HString/HString.hpp"
#include"../../include/extern_onlineuser.hpp"
#include<algorithm>
#include<memory>
#include<string>
#include<unordered_map>
#include<mutex>
#include<cstring>

using namespace ceh::String;
using namespace std;
using namespace serverData;


void
doSendFriendsList(serviceInfo* info)
{
    auto friendsData = get_userFriends(info->userid);
    auto friendsVec = *friendsData;
    size_t number = friendsVec.size();
    wchar_t* numberStr = new wchar_t[16];
    memset(numberStr, 0, 16);
    uintToWStr(number, numberStr);

    easySendMsg(info->userfd, make_shared<wstring>(L"GETFRIENDSLIST_OK"));//SendHead
    easySend_WString(info->userfd, make_shared<wstring>(numberStr));//Send Number
    //Send loop
    {
        unique_lock lock(*onlineUsersMutexLock);
        for(size_t i = 0; i < number; ++i)
        {
            auto idnum = stoul(friendsVec[i]);
            auto id = get_userID(idnum); 
            auto name = get_userName(idnum); 
            wstring statusStr;
            bool cont = onlineUsers->contains(stoi(friendsVec[i]));

            if(cont)
                statusStr = L"ON";
            else
                statusStr = L"OFF";
            auto status = make_shared<wstring>(statusStr); 
            auto intro = get_userIntro(idnum); 
            auto profile = get_userProfile(idnum); 

            easySend_WString(info->userfd, id); 
            easySend_WString(info->userfd, name); 
            easySend_WString(info->userfd, status); 
            easySend_WString(info->userfd, intro); 
            easySend_File(info->userfd, profile->data, profile->bytes); 
        }
    }
    showMinior(L"FINISH",L"PROG: get friend list.");
    delete[] numberStr;
}