#include"userdatabase.hpp"
#include"convert.hpp"
#include"../../lib/Json/json.hpp"
#include"../service/service.hpp"
#include"../server/guard.hpp"
#include"../../lib/HTime/HTime.hpp"

#include"../server/standard.hpp"

#include<algorithm>
#include<fstream>

using namespace std;
using namespace guard;
using namespace ceh::Time;
using json = nlohmann::json;



    auto& UserDataBase::getItem(userID_t userID)
    {
        uint index = userID - standard::userid_begin;
        try
        {
            return (*j)[index];
        }
        catch(json::out_of_range& e)
        {
            this->newItem(userID);
            errorExit(L"[Error] UserDataBase.getItem() : out_of_range");
            return  (*j)[index];
        }
        catch(json::other_error& e)
        {
            errorExit(L"[Error] UserDataBase.getItem() : other_error");
            return (*j)[index];
        }
    }

    json UserDataBase::getRawData()
    {
        return *j;
    }

    UserDataBase::UserDataBase(){
        in = new ifstream(standard::userDataBase_Name);
        try
        {
            j = new json(json::parse(*in));
        }
        catch(json::parse_error& e)
        {
            errorMsg(L"[UserDataBase] json parse error , already create new\n");
            j  = new json();
        }
        
    }
    UserDataBase::~UserDataBase()
    {
        // save and delete 
        in->close();
        delete in;

        ofstream* out = new ofstream(standard::userDataBase_Name);
        (*out) << setw(4) << (*j);
        out->close();
        delete out;
        delete j;
    }
    uint UserDataBase::getSize()const
    {
        monitor(L"Tracl UserDataBase -> getSize()");
        return j->size();
    }
    
    void UserDataBase::newItem(userID_t userID)
    {
        uint index = userID - standard::userid_begin;
        
        auto& newItem = (*j)[index];

        newItem["name"];
        newItem["password"];
        newItem["id"] = userID;
        newItem["reg-date"];
        newItem["friends"];
        newItem["intro"];
        newItem["email"];
    }

    void UserDataBase::setItem_name(userID_t userid, shared_ptr<wstring> name)
    {
        auto& item = getItem(userid);
        item["name"] = wstring_to_utf8(*name);
    }
    void UserDataBase::setItem_password(userID_t userid,shared_ptr<wstring> password)
    {
        auto &item = getItem(userid);
        item["password"] = wstring_to_utf8(*password);
    }
    void UserDataBase::setItem_regDateToday(userID_t userid)
    {
        HTime* t = new HTime;
        auto &item = getItem(userid);
        item["reg-date"] = {t->getYearByInt(), t->getMonthByInt(), t->getMDayByInt()};
        delete t;
    }
    void UserDataBase::setItem_intro(userID_t userid, shared_ptr<wstring> intro)
    {
        auto &item = getItem(userid);
        item["intro"] = wstring_to_utf8(*intro);
    }
    void UserDataBase::setItem_email(userID_t userid, shared_ptr<wstring> email)
    {
        auto& item = getItem(userid);
        item["email"] = wstring_to_utf8(*email);
    }

    wstring UserDataBase::getItem_name(userID_t userid)
    {
        auto& item = getItem(userid);
        return utf8_to_wstring(item["name"]);
    }
    wstring UserDataBase::getItem_password(userID_t userid)
    {
        auto &item = getItem(userid);
        return utf8_to_wstring(item["password"]);
    }
    vector<wstring> UserDataBase::getItem_friends(userID_t userid)
    {
        auto &item = getItem(userid);
        vector<string> fs;
        vector<wstring> res;
        
        if(item["friends"].is_null())
            return res;
        else
            fs = item["friends"];

        for(auto f : fs)
            res.push_back(utf8_to_wstring(f));
        return res;
    }
    wstring UserDataBase::getItem_intro(userID_t userid)
    {
        auto& item = getItem(userid);
        return utf8_to_wstring(item["intro"]);
    }

    bool UserDataBase::containItem_name(std::shared_ptr<std::wstring> name)
    {
        bool res = false;
        for(auto& item : (*j))
        {
            res = (item["name"] == wstring_to_utf8(*name));
            if(res) break;
        }
        return res;
    }
    bool UserDataBase::containItem_email(std::shared_ptr<std::wstring> email)
    {
        bool res = false;
        for(auto& item : (*j))
        {
            res = (item["email"] == wstring_to_utf8(*email));
            if(res) break;
        }
        return res;
    }

    void UserDataBase::addItem_friends(userID_t userid, shared_ptr<wstring> newFriendIDStr)
    {
        string newf = wstring_to_utf8(*newFriendIDStr);
        auto& item = getItem(userid);
        item["friends"].push_back(newf);
    }

    void UserDataBase::removeItem_friends(userID_t userid, shared_ptr<wstring> removeIDStr)
    {
        string ref = wstring_to_utf8(*removeIDStr);
        auto& item = getItem(userid);
        auto& fs = item["friends"];

        for(auto iter = fs.begin(); iter != fs.end(); ++iter)
        {
            if(ref == *iter)
            {
                fs.erase(iter);
                break;
            }
        }
    }

