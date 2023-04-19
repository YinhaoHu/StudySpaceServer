#include"friend.hpp"
#include"userdata.hpp"
#include"notice.hpp"
#include"../server/net.hpp"
#include"../server/guard.hpp"
#include"../../include/extern_onlineuser.hpp"

#include<algorithm>
#include<unordered_set>

using namespace guard;
using namespace serverData;
using namespace std;

//used to control user to request to add new friend only once.
unordered_map<userID_t, unordered_set<userID_t>>* requestFriend_Limit
 = new unordered_map<userID_t, unordered_set<userID_t>>;


//Just finish the request basic check
void doAddFriendRequest(serviceInfo* info)
{
    auto friendID = easyRecv_WString(info->userfd);
    uint friendIDnum = stoi(*friendID);
    auto friendList = get_userFriends(info->userid);
    wstring* response = new wstring;

    if(check_userExist(friendIDnum) == false)
        *response = L"NONEXIST";
    else if(friendList->end() != find_if(friendList->begin(), friendList->end(),
                [friendID](wstring& x){return x.compare(*friendID)==0;}) )
        *response = L"ALREADY";
    else if((*requestFriend_Limit)[info->userid].contains(friendIDnum))
        *response = L"REQUESTED";
    else 
        *response = L"OK";
    
    //send order:1 eventType 2 response
    easySendMsg(info->userfd, make_shared<wstring>(L"ADDFRIEND_REQUEST"));
    easySend_WString(info->userfd, make_shared<wstring>(*response));

    //send to Friend part
    //Notification prog: content ADDFRIEND_REQUEST USERNAME ID
    if(response->compare(L"OK") == 0)
    {
        wstring* msg = new wstring(L"ADDFRIEND_REQUEST ");
        msg->append(*serverData::get_userName(info->userid) + L' ');
        msg->append(*serverData::get_userID(info->userid)); 
        noticeUser(friendIDnum, make_shared<wstring>(*msg));
        (*requestFriend_Limit)[info->userid].insert(friendIDnum);
        delete msg;
    }
    showMinior(L"FINISH", L"Add friend request");
    delete response;
}

void doAddFriendResponse(serviceInfo* info)//info: who was asked to be friend
{
    /*
        RECV ORDER:
        1) REJECT / ACCEPT
        2) requester id

        SEND ORDER
        PROG: NOTICE
        ARGS: ADDFRIEND_CONFIRM FROMID ACCEPT/REJECT
        //IF ACCEPT -> SEND PROFILE NAME ONLINESTATUS
    */
    auto response = easyRecv_WString(info->userfd);
    auto requester = easyRecv_WString(info->userfd);

    //SEND
    auto res = std::make_shared<wstring>(L"ADDFRIEND_CONFIRM ");
    res->append(info->userid_str);
    res->push_back(L' ');
    if(response->compare(L"ACCEPT") == 0)
    {    
        serverData::add_userFriend(info->userid, requester);
        serverData::add_userFriend(stoul(*requester), std::make_shared<wstring>(info->userid_str));
        res->append(L"ACCEPT");
    }
    else
        res->append(L"REJECT");
    requestFriend_Limit->at(stoul(*requester)).erase(info->userid);
    noticeUser(stoul(*requester), res);
    showMinior(L"FINISH",L"ADD CONFIRM");
}

void doGetNewFriendInfo(serviceInfo* info)
{
    auto friendID = easyRecv_WString(info->userfd);

    auto friendIDNum = stoul(*friendID);
    auto profile = serverData::get_userProfile(friendIDNum);
    auto onlineStatus = onlineUsers->contains(friendIDNum) ? wstring(L"ON") : wstring(L"OFF");
    /*
    Send order:
    0 prog
    1 profile
    2 username
    3 intro
    4 onlinestatus
    5 userid
    */
    easySendMsg(info->userfd, std::make_shared<wstring>(L"LOAD_NEW_FRIEND_INFO"));
    easySend_File(info->userfd, profile->data, profile->bytes);
    easySend_WString(info->userfd, serverData::get_userName(friendIDNum));
    easySend_WString(info->userfd, serverData::get_userIntro(friendIDNum));
    easySend_WString(info->userfd, std::make_shared<wstring>(onlineStatus));
    easySend_WString(info->userfd, friendID);

    showMinior(L"FINISH", L"do send new friend info");
}

void doRemovoeFriendRequest(serviceInfo* info)
{
    auto userIDStr_ToRemove = easyRecv_WString(info->userfd);
    auto userIDNum_toRemove = stoul(*userIDStr_ToRemove);

    remove_userFriend(info->userid, userIDStr_ToRemove);
    remove_userFriend(userIDNum_toRemove, make_shared<wstring>(info->userid_str));

    noticeUser(info->userid, 
        make_shared<wstring>(L"REMOVE_FRIEND "+*userIDStr_ToRemove));
    noticeUser(userIDNum_toRemove, 
        make_shared<wstring>(L"REMOVE_FRIEND "+wstring(info->userid_str)));
}