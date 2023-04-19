#include"userinfo_change.hpp"

#include"../server/standard.hpp"
#include"../server/net.hpp"
#include"../../lib/HData/HData.hpp"
#include"userdata.hpp"

#include<memory>
#include<cstring>

using namespace std;
using namespace ceh::Data;

void do_userinfo_change_getOldPassword(serviceInfo* info)
{
    HWData* userinfo = new HWData(standard::userDataFile);
    userinfo->load();
    HWDataItem& this_user_info = (*userinfo)[info->userid - standard::userid_begin];

    HWDataItem_value& password_str =this_user_info.values[standard::userDataField::password];
    wchar_t* oldpassword = new wchar_t[maxSockBufferSize];
    char* sentmsg = new char[maxSockBufferBytes];

    memset(oldpassword, 0, maxSockBufferBytes);
    wcscpy(oldpassword, L"USERINFO_CHANGE_OLDPASSWORD");
    memcpy(sentmsg, oldpassword, maxSockBufferBytes);
    send(info->userfd, sentmsg, maxSockBufferBytes, 0);

    memset(oldpassword,0 , maxSockBufferBytes);
    wcscpy(oldpassword, password_str.c_str());
    memcpy(sentmsg, oldpassword, maxSockBufferBytes);
    send(info->userfd, sentmsg, maxSockBufferBytes, 0);
    
    userinfo->save();
    delete[] sentmsg;
    delete[] oldpassword;
    delete userinfo;
    showMinior(L"FINISH", L"DO_USERINFO_CHANGE:  GETOLDPASSWORD");
}

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
    serverData::modify_userProfile(info->userid, fileNode->data, fileNode->bytes);
    serverData::modify_userIntro(info->userid, recv5_intro_);
    serverData::modify_userPassword(info->userid, make_shared<wstring>(recv4_password));
    serverData::modify_userName(info->userid, make_shared<wstring>(recv3_name));

    char32_t* sendBack_okResponse =new char32_t[maxSockBufferSize]; 
    memset(sendBack_okResponse, 0, maxSockBufferBytes);
    memcpy(sendBack_okResponse, L"USERINFO_CHANGE_OK", 19 * sizeof(wchar_t));
    send(info->userfd, sendBack_okResponse, maxSockBufferBytes,0);

    //change successfully.
    showMinior(L"SEND", L"CHANGE USERINFO");
    delete[] recv3_name;
    delete[] recv4_password;
    delete[] recv5_intro;
    delete[] sendBack_okResponse;
}