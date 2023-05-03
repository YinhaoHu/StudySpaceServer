#include"userinfo_change.hpp"
#include"../server/guard.hpp"
#include"../server/standard.hpp"
#include"../server/net.hpp"
#include"../../lib/HData/HData.hpp"
#include"../../include/extern_onlineuser.hpp"
#include"userdata.hpp"

#include<memory>
#include<cstring>

using namespace std;
using namespace ceh::Data;

void do_userinfo_change_getOldPassword(serviceInfo* info)
{
    /*
            This code responses to  ' USERINFO_CHANGE_GET_OLDPASSWORD '
        Response head: USERINFO_CHANGE_OLDPASSWORD
        Response line: password
    */
    
    easySendMsg(info->userfd, make_shared<wstring>(L"USERINFO_CHANGE_OLDPASSWORD"));
    easySendMsg(info->userfd, data::user::get_userPassword(info->userid));//For compatible

    guard::monitor(L"FINISH", L"DO_USERINFO_CHANGE:  GETOLDPASSWORD");
}

static inline void broadcast_friend_newInfo(serviceInfo* info);

void do_changeUserInfo(serviceInfo* info)
{
    //--3~5
    wchar_t* recv3_name = new wchar_t[maxSockBufferSize];
    wchar_t* recv4_password = new wchar_t[maxSockBufferSize];
    wchar_t* recv5_intro  = new wchar_t[maxSockBufferSize];

    memset(recv3_name, 0, maxSockBufferBytes);
    memset(recv4_password, 0, maxSockBufferBytes);
    memset(recv5_intro, 0, maxSockBufferBytes);
    
    recv(info->userfd, recv3_name, maxSockBufferBytes, MSG_WAITALL);
    recv(info->userfd, recv4_password, maxSockBufferBytes, MSG_WAITALL);
    //recv(info->userfd, recv5_intro, maxSockBufferBytes, MSG_WAITALL);
    auto recv5_intro_ = easyRecv_WString(info->userfd);
    auto fileNode = easyRecv_File(info->userfd);

    //change corresponding data. { name, password, intro, profile}.
    data::user::modify_userProfile(info->userid, fileNode->data, fileNode->bytes);
    data::user::modify_userIntro(info->userid, recv5_intro_);
    data::user::modify_userPassword(info->userid, make_shared<wstring>(recv4_password));
    data::user::modify_userName(info->userid, make_shared<wstring>(recv3_name));

    char32_t* sendBack_okResponse =new char32_t[maxSockBufferSize]; 
    memset(sendBack_okResponse, 0, maxSockBufferBytes);
    memcpy(sendBack_okResponse, L"USERINFO_CHANGE_OK", 19 * sizeof(wchar_t));
    send(info->userfd, sendBack_okResponse, maxSockBufferBytes,0);

    //change successfully.
    guard::monitor(L"SEND", L"CHANGE USERINFO");

    //broad cast.
    broadcast_friend_newInfo(info);
    delete[] recv3_name;
    delete[] recv4_password;
    delete[] recv5_intro;
    delete[] sendBack_okResponse;
}

static inline void broadcast_friend_newInfo(serviceInfo* info)
{
    {
        unique_lock lock(*onlineUsersMutexLock);

        auto friends = data::user::get_userFriends(info->userid);
        {
            for(auto f : *friends)
            {
                auto fid = stoul(f);
                if(onlineUsers->contains(fid))
                {
                    auto fsock = onlineUsers->at(fid);
                    auto profile = data::user::get_userProfile(info->userid);
                    auto senderID = make_shared<wstring>(info->userid_str);

                    easySendMsg(fsock, make_shared<wstring>(L"FRIEND_NEW_PROFILE"));
                    easySend_WString(fsock, senderID);
                    easySend_File(fsock, profile->data, profile->bytes);

                    easySendMsg(fsock, make_shared<wstring>(L"FRIEND_NEW_NAME"));
                    easySend_WString(fsock, senderID);
                    easySend_WString(fsock, data::user::get_userName(info->userid));

                    easySendMsg(fsock, make_shared<wstring>(L"FRIEND_NEW_INTRO"));
                    easySend_WString(fsock, senderID);
                    easySend_WString(fsock, data::user::get_userIntro(info->userid));
                }
            }
        }
    }
}