#pragma once

#include<memory>
#include<fstream>
#include<vector>
#include"../server/monitor.hpp"
#include"../../lib/Json/json.hpp"
#include"../service/service.hpp"

class UserDataBase
{
friend class Monitor;

private:
    std::ifstream *in;
    nlohmann::json *j;

    auto& getItem(userID_t userID);

protected:
    nlohmann::json getRawData();

public: 
    UserDataBase();
    ~UserDataBase();
    uint getSize()const;

    void newItem(userID_t userID);

    void setItem_name(userID_t userid, std::shared_ptr<std::wstring> name);
    void setItem_password(userID_t userid,std::shared_ptr<std::wstring> password);
    void setItem_regDateToday(userID_t userid);
    void setItem_intro(userID_t userid, std::shared_ptr<std::wstring> intro);
    void setItem_email(userID_t userid, std::shared_ptr<std::wstring> email);
    
    std::wstring getItem_name(userID_t userid);
    std::wstring getItem_password(userID_t userid);
    std::vector<std::wstring> getItem_friends(userID_t userid);
    std::wstring getItem_intro(userID_t userid);

    bool containItem_name(std::shared_ptr<std::wstring> name);
    bool containItem_email(std::shared_ptr<std::wstring> email);

    void addItem_friends(userID_t userid, std::shared_ptr<std::wstring> newFriendIDStr);

    void removeItem_friends(userID_t userid, std::shared_ptr<std::wstring> removeIDStr);
};