//register.cpp: support the REGISTER service
/*
---------------------------------------------------------------------
                        + INFORMATION +

    1. The available user ID : 10001(1e5+1)  ~  999999999999(1e13-1)
    2. The format of userdata field:
        ID[KEY] USERNAME[0] PASSWORD[1] EMAIL[2] REG-YEAR[3] 
        REG-MONTH[4] REG-DAY[5]
        
---------------------------------------------------------------------
*/
#include"register.hpp"

#include"../data/serverdata.hpp"
#include"../server/net.hpp"
#include"../server/guard.hpp"
#include"../server/standard.hpp"
#include"../../lib/HString/HString.hpp"
#include"../../lib/HData/HData.hpp"
#include"../data/userdata.hpp"

#include<sys/socket.h>
#include<sys/stat.h>
#include<filesystem>
#include<algorithm>
#include<string>

#include<cwchar>
#include<cstring>

using namespace std;
using namespace ceh::Data;
using namespace ceh::String;

const char* registerFile = "data/users.hdat";

struct RegisterInfo{
    wchar_t username[maxFieldSize];
    wchar_t password[maxFieldSize];
    wchar_t code[maxFieldSize];
    wchar_t email[maxFieldSize];
    wchar_t year[maxFieldSize];
    wchar_t month[maxFieldSize];
    wchar_t day[maxFieldSize];
    wchar_t allocatedId[maxFieldSize];
};

static void parseInfo(serviceInfo* info, RegisterInfo* result);
static int checkInfo(RegisterInfo* info);
static  void generateUserData(RegisterInfo* info);

void doRegister(serviceInfo* info)
{
    wstring sendbuf;
    int status;
    RegisterInfo registerinfo;

    parseInfo(info, &registerinfo);
    status = checkInfo(&registerinfo);
    
    switch(status)
    {    
        case 0: 
            sendbuf.append(L"OK ") ;
            sendbuf.append(registerinfo.allocatedId);
            break;
        case 1: sendbuf = L"EMAIL";break;
        case 2: sendbuf = L"NAME";break;
        case 3: sendbuf = L"CODE";break;
        default: sendbuf = L"UNKNOWN";
    }
    generateUserData(&registerinfo);
    easySend_WString(info->userfd, make_shared<wstring>(sendbuf));
}

static void parseInfo(serviceInfo* info, RegisterInfo* result)
{
    wchar_t delimeter,*first, *second;
    wchar_t* third, *fourth, *fifth, *sixth;

    delimeter = L' ';
    first = findwchr(info->args, delimeter);
    second = findwchr(first+1, delimeter);
    third = findwchr(second+1, delimeter);
    fourth = findwchr(third+1, delimeter);
    fifth = findwchr(fourth+1, delimeter);
    sixth = findwchr(fifth+1, delimeter);

    *first = L'\0';
    *second = L'\0';
    *third = L'\0';
    *fourth = L'\0';
    *fifth = L'\0';
    *sixth = L'\0';

    wcscpy(result->username, info->args);
    wcscpy(result->password, first+1);
    wcscpy(result->code , second+1);
    wcscpy(result->email , third+1);
    wcscpy(result->year , fourth+1);
    wcscpy(result->month , fifth+1);
    wcscpy(result->day, sixth+1);
}

static int checkInfo(RegisterInfo* info)
{
    if(data::user::exist_email(make_shared<wstring>(info->email)))
        return 1;
    else if(data::user::exist_name(make_shared<wstring>(info->username)))
        return 2;
    else if(data::server::get_registerCode()->compare(info->code) != 0)
        return 3;
    
    auto  newID= data::user::get_userNumber() + standard::userid_begin;
    memset(info->allocatedId, 0, sizeof(info->allocatedId));
    memcpy(info->allocatedId, to_wstring(newID).c_str(), 
        to_wstring(newID).length() * sizeof(wchar_t));

    return 0;
}

static  void generateUserData(RegisterInfo* info)
{
    auto idnum = stoul(info->allocatedId);
    data::user::init_userdata(idnum,
            make_shared<wstring>(info->username), 
            make_shared<wstring>(info->password),
            make_shared<wstring>(info->email));

    string from = standard::dataFileDir;
    from.append(standard::defaultprofile_name);

    string to = standard::userProfileDir;
    to.append(to_string(idnum));

    filesystem::copy_file(from, to);
    guard::monitor(L"Generate User Data", info->allocatedId);
}