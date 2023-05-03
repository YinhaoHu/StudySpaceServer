#include"userdata.hpp"
#include"userdatabase.hpp"
#include"../server/standard.hpp"
#include"../server/guard.hpp"
#include"../server/net.hpp"

#include"../../lib/HTime/HTime.hpp"
#include"../../lib/Json/json.hpp"

#include<algorithm>
#include<string>
#include<vector>
#include<fstream>
#include<memory>
#include<codecvt>
#include<locale>

#include<cstring>
#include<cstdio>

#include<sys/stat.h>

using namespace guard;
using namespace data::user;
using namespace std;
using namespace ceh::Time;

using wstring_sptr = std::shared_ptr<std::wstring> ;
using char_sptr = std::shared_ptr<char>;
using file_sptr = std::shared_ptr<FILE>;
using json = nlohmann::json;

static inline char_sptr helper_getFilename(userID_t id, const char* direcory);

    //init
void data::user::init_userdata(userID_t userid, std::shared_ptr<wstring> name, 
        std::shared_ptr<wstring> password,std::shared_ptr<wstring> email)
{
    auto db = new UserDataBase;
    db->newItem(userid);
    db->setItem_name(userid, name);
    db->setItem_password(userid, password);
    db->setItem_email(userid, email);
    db->setItem_regDateToday(userid);
    db->setItem_intro(userid, std::make_shared<wstring>(L"StudySpace 用户"));
    
    delete db;
}
    //check

bool data::user::check_userExist(userID_t id)
{
    auto idx = id - standard::userid_begin;

    auto db = new UserDataBase;
    bool res = (idx < db->getSize());

    delete db;
    return res;
}

    //Get

std::shared_ptr<std::wstring> data::user::get_userPassword(userID_t id)
{
    auto udb = new UserDataBase;
    auto res = udb->getItem_password(id);
    delete udb;
    return make_shared<wstring>(res);
}

uint data::user::get_userNumber()
{
    auto udb = new UserDataBase;
    auto res = udb->getSize();
    delete udb;
    return res;
}

std::shared_ptr<std::wstring>
data::user::get_userID(userID_t id)
{
    //for compatiable
    guard::monitor(L"Track -> data::user::get_userID", to_wstring(id).c_str());
    return make_shared<wstring>(to_wstring(id));
}

std::shared_ptr<std::wstring> 
data::user::get_userIntro(userID_t id)
{
    auto udb = new UserDataBase;
    auto res = udb->getItem_intro(id);
    delete udb;
    guard::monitor(L"Track -> data::user::get_userIntro", res.c_str());
    return make_shared<wstring>(res);
}

std::shared_ptr<std::wstring> data::user::
get_userName(userID_t id)
{
    auto udb = new UserDataBase;
    auto res = udb->getItem_name(id);
    delete udb;
    guard::monitor(L"Track -> data::user::get_username", res.c_str());
    return make_shared<wstring>(res);
}

std::shared_ptr<std::wstring> 
get_userEMail(userID_t id)
{
    throw "Not implemented";
return make_shared<wstring>();
}

std::shared_ptr<std::wstring> 
get_userPassword(userID_t id)
{
    auto udb = new UserDataBase;
    auto res = udb->getItem_password(id);
    delete udb;

    guard::monitor(L"Track -> data::user::get_userPassword", res.c_str());
    return make_shared<wstring>(res);
}

std::shared_ptr<data::user::DataNode> data::user::
get_userProfile(userID_t id)
{
    auto filename = helper_getFilename(id, standard::userProfileDir);
    char* filedata;
    size_t filesize;

    FILE* file = fopen(filename.get(), "rb");
    struct stat filestat;
    memset(&filestat,0,sizeof(struct stat));

    if(stat(filename.get(), &filestat) == -1)
        errorMsg(L"[Error] get_userprofile: stat");

    filesize = filestat.st_size;
    filedata = new char[filesize];
    memset(filedata,0,filesize);
    fread(filedata, sizeof(char), filesize, file);
    fclose(file);
    guard::monitor(L"Track -> data::user::get_userProfile (filesize)", to_wstring(filesize).c_str());
    return make_shared<data::user::DataNode>(filesize, filedata);
}

std::shared_ptr<std::vector<std::wstring>> data::user::
get_userFriends(userID_t id)
{
    auto udb = new UserDataBase;
    auto res = udb->getItem_friends(id);
    guard::monitor(L"Track(UserDataBase) -> get_userFriends- number", to_wstring(res.size()).c_str());
    delete udb;
    return make_shared<vector<wstring>>(res);
 }

    //Modify
bool 
data::user::modify_userIntro(userID_t id,std::shared_ptr<std::wstring> newIntro)
{
    auto udb = new UserDataBase;
    udb->setItem_intro(id , newIntro);
    delete udb;
    return true;
}

bool 
data::user::modify_userName(userID_t id,std::shared_ptr<std::wstring> newUsername)
{
    auto udb = new UserDataBase;
    udb->setItem_name(id , newUsername);
    delete udb;
    return true;
}

bool 
modify_userEMail(userID_t id,std::shared_ptr<std::wstring> newEMail)
{
    auto udb = new UserDataBase;
    udb->setItem_email(id , newEMail);
    delete udb;
    return true;
}

bool 
data::user::modify_userPassword(userID_t id, std::shared_ptr<std::wstring> newPassword)
{
    auto udb = new UserDataBase;
    udb->setItem_password(id , newPassword);
    delete udb;
    return true;
}

bool 
data::user::modify_userProfile(userID_t id, std::shared_ptr<char> newProfile, size_t filesize)
{
    char* filename = new char[64];
    
    memset(filename, 0, 64);
    
    strcpy(filename, standard::userProfileDir);
    strcat(filename, to_string(id).c_str());

    FILE* file = fopen(filename, "wb");
    fwrite(newProfile.get(), sizeof(char), filesize, file);
    fclose(file);

    delete[] filename;
    return true;
}

    //Add

bool data::user::add_userIntro(userID_t id,std::shared_ptr<std::wstring> newIntro)
{
    auto udb = new UserDataBase;
    udb->setItem_intro(id , newIntro);
    delete udb;
    return true;
}


bool 
data::user::add_userInfo(userID_t id,std::shared_ptr<std::vector<std::wstring>> info )
{
    return true;
}

bool data::user::add_userFriend(userID_t id, std::shared_ptr<std::wstring> addedID)
{
    auto udb = new UserDataBase;
    udb->addItem_friends(id , addedID);
    delete udb;
    return true;
}

bool data::user::exist_email(std::shared_ptr<wstring> email)
{
    auto udb = new UserDataBase;
    bool res = udb->containItem_email(email);
    delete udb;
    return res;
}

bool data::user::exist_name(shared_ptr<wstring> name)
{
    auto udb = new UserDataBase;
    bool res = udb->containItem_name(name);
    delete udb;
    return res;
}

bool data::user::exist_id(uint id)
{
    return id < data::user::get_userNumber() + standard::userid_begin;
}

bool data::user::
remove_userFriend(userID_t id, std::shared_ptr<std::wstring> removedID)
{
    auto udb = new UserDataBase;
    udb->removeItem_friends(id, removedID);
    delete udb;
    return true;
}

//Helper.
static inline char_sptr helper_getFilename(userID_t id, const char* direcory)
{
    char_sptr filename(new char[64]);

    memset(filename.get(), 0, 64);

    strcpy(filename.get(), direcory);
    strcat(filename.get(), to_string(id).c_str());

    return filename;
}