#include"userdata.hpp"
#include"../server/standard.hpp"
#include"../server/guard.hpp"
#include"../../lib/HString/HString.hpp"
#include"../../lib/HData/HData.hpp"

#include<algorithm>
#include<string>
#include<vector>
#include<fstream>
#include<memory>

#include<cstring>
#include<cstdio>

#include<sys/stat.h>

using namespace guard;
using namespace serverData;
using namespace std;
using namespace ceh::String;
using namespace ceh::Data;

using wstring_sptr = std::shared_ptr<std::wstring> ;
using char_sptr = std::shared_ptr<char>;
using file_sptr = std::shared_ptr<FILE>;
using hwdataitem_sptr = std::shared_ptr<HWDataItem>;


static inline char_sptr helper_getFilename(userID_t id, const char* direcory);

    //init
bool serverData::
init_userfriend(userID_t id)
{
    wchar_t* idStr = new wchar_t[16];

    memset(idStr, 0, 16 * sizeof(wchar_t));
    uintToWStr(id, idStr);

    HWData* data = new HWData(standard::userFriendsListFile);
    HWDataItem* newItem = new HWDataItem(HWDataItem_key(idStr), HWDataItem_values());
    data->load();
    data->append(*newItem);
    data->save();
    delete newItem;
    delete data;
    delete[] idStr;
    return true;
}
    //check

bool serverData::check_userExist(userID_t id)
{
    auto idx = id - standard::userid_begin;
    bool res;
    
    HWData* userdata = new HWData(standard::userDataFile);
    userdata->load();
    res = (idx < userdata->size());
    userdata->save();
    delete userdata;
    
    return res;
}

    //Get
std::shared_ptr<std::wstring>
serverData::get_userID(userID_t id)
{
    id = id - standard::userid_begin;
    HWData* userdata = new HWData(standard::userDataFile);
    userdata->load();
    auto item = (*userdata)[id].key;
    auto res  = make_shared<wstring>(item);
    userdata->save();
    delete userdata;
    return res;
}

std::shared_ptr<std::wstring> 
serverData::get_userIntro(userID_t id)
{
    auto filename = helper_getFilename(id, standard::userIntroDir);
    wifstream* in = new wifstream(filename.get(), ios::in);
    wstring* intro = new wstring, *temp = new wstring;

    in->imbue(std::locale("en_US.UTF-8"));
    while(  ((*in) >> (*temp)).eof() == false)
    {
        intro->append(*temp);
        intro->push_back(L' ');
    }
    if(temp->empty() == false)
        intro->append(*temp);

    in->close();
    delete in;
    delete temp;
    return shared_ptr<wstring>(intro);
}

std::shared_ptr<std::wstring> serverData::
get_userName(userID_t id)
{
    id = id - standard::userid_begin;
    HWData* userdata = new HWData(standard::userDataFile);
    userdata->load();
    auto item = (*userdata)[id].values[standard::userDataField::username];
    auto res  = make_shared<wstring>(item);
    userdata->save();
    delete userdata;
    return res;
}

std::shared_ptr<std::wstring> 
get_userEMail(userID_t id)
{
return make_shared<wstring>();
}

std::shared_ptr<std::wstring> 
get_userPassword(userID_t id)
{
    return make_shared<wstring>();
}

std::shared_ptr<serverData::DataNode> serverData::
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

    return make_shared<serverData::DataNode>(filesize, filedata);
}

std::shared_ptr<std::vector<std::wstring>> serverData::
get_userFriends(userID_t id)
{
    id = id - standard::userid_begin;
    HWData* data = new HWData(standard::userFriendsListFile);

    data->load();
    auto item = (*data)[id].values;
    data->save();
    delete data;
    return std::make_shared<vector<wstring>>(item);
 }

    //Modify
bool 
serverData::modify_userIntro(userID_t id,std::shared_ptr<std::wstring> newIntro)
{
    char_sptr filename = helper_getFilename(id, standard::userIntroDir);

    wofstream* out = new wofstream(filename.get(), std::ios::out);
    out->imbue(std::locale("en_US.UTF-8"));
    (*out) << *newIntro;
    out->close();

    delete out;
    return true;
}

bool 
serverData::modify_userName(userID_t id,std::shared_ptr<std::wstring> newUsername)
{
    id = id - standard::userid_begin;

    HWData* userdatas= new HWData(standard::userDataFile);
    userdatas->load();

    (*userdatas)[id].values[standard::userDataField::username] = *newUsername;
 
    userdatas->save();
    delete userdatas;
    return true;
}

bool 
modify_userEMail(userID_t id,std::shared_ptr<std::wstring> newEMail)
{
return true;
}

bool 
serverData::modify_userPassword(userID_t id, std::shared_ptr<std::wstring> newPassword)
{
    id = id - standard::userid_begin;

    HWData* userdatas= new HWData(standard::userDataFile);
    userdatas->load();

    (*userdatas)[id].values[standard::userDataField::password] = *newPassword;
 
    userdatas->save();
    delete userdatas;
    return true;
}

bool 
serverData::modify_userProfile(userID_t id, std::shared_ptr<char> newProfile, size_t filesize)
{
    char* id_str = new char[16];
    char* filename = new char[64];
    
    memset(id_str, 0, 16);
    memset(filename, 0, 64);
    
    uintToStr(id, id_str);
    strcpy(filename, standard::userProfileDir);
    strcat(filename, id_str);

    FILE* file = fopen(filename, "wb");
    fwrite(newProfile.get(), sizeof(char), filesize, file);
    fclose(file);

    delete[] id_str;
    delete[] filename;
    return true;
}

    //Add

bool 
serverData::add_userIntro(userID_t id,std::shared_ptr<std::wstring> newIntro)
{
    auto filename = helper_getFilename(id, standard::userIntroDir);
    wofstream* out = new wofstream(filename.get(), ios::out);

    out->imbue(std::locale("en_US.UTF-8"));
    out->write(newIntro->c_str(), newIntro->size() );

    out->close();
    delete out;
    
    return true;
}


bool 
serverData::add_userInfo(userID_t id,std::shared_ptr<std::vector<std::wstring>> info )
{
    return true;
}

bool serverData::
add_userFriend(userID_t id, std::shared_ptr<std::wstring> addedID)
{
    id = id - standard::userid_begin;

    HWData* data = new HWData(standard::userFriendsListFile);
    data->load();
    (*data)[id].values.push_back(*addedID);
    data->save();
    delete data;

    return true;
}


bool serverData::
remove_userFriend(userID_t id, std::shared_ptr<std::wstring> removedID)
{
    id = id - standard::userid_begin;

    bool res = false;
    HWData* data = new HWData(standard::userFriendsListFile);
    data->load();

    auto& item = (*data)[id].values;
    std::erase_if(item, [removedID](wstring& x){ return x.compare(*removedID) == 0;});

    data->save();
    delete data;

    return res;
}


//Helper.
static inline char_sptr helper_getFilename(userID_t id, const char* direcory)
{
    char_sptr filename(new char[64]);
    char userid_str[16];

    memset(userid_str, 0, 16);
    memset(filename.get(), 0, 64);

    uintToStr(id, userid_str);
    strcpy(filename.get(), direcory);
    strcat(filename.get(), userid_str);

    return filename;
}

