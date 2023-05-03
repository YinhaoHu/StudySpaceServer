#pragma once

// The high level wrapper of user data process interface
#include"../service/service.hpp"

#include<memory>
#include<string>
#include<vector>

namespace data
{
    namespace user
    {
    struct DataNode
    {
    size_t bytes;
    std::shared_ptr<char> data;

    DataNode(size_t _bytes = 0, char* _data = nullptr):
        bytes(_bytes), data(std::shared_ptr<char>(_data))
        {}
    ~DataNode(){}
    };

    void init_userdata(userID_t id, std::shared_ptr<std::wstring> name, 
            std::shared_ptr<std::wstring> password, std::shared_ptr<std::wstring> email);
    bool check_userExist(userID_t id);

    uint get_userNumber();
    std::shared_ptr<std::wstring> get_userID(userID_t id);
    std::shared_ptr<std::wstring> get_userIntro(userID_t id);
    std::shared_ptr<std::wstring> get_userName(userID_t id);
    std::shared_ptr<std::wstring> get_userEMail(userID_t id);
    std::shared_ptr<std::wstring> get_userPassword(userID_t id);
    std::shared_ptr<DataNode> get_userProfile(userID_t id);//No change 
    std::shared_ptr<std::vector<std::wstring>> get_userFriends(userID_t id);

    bool modify_userIntro(userID_t id,std::shared_ptr<std::wstring> newIntro);
    bool modify_userName(userID_t id,std::shared_ptr<std::wstring> newUsername);
    bool modify_userEMail(userID_t id,std::shared_ptr<std::wstring> newEMail);
    bool modify_userPassword(userID_t id, std::shared_ptr<std::wstring> newPassword);
    bool modify_userProfile(userID_t id, std::shared_ptr<char> newProfile, size_t filesize); //Nochange

    bool add_userIntro(userID_t id,std::shared_ptr<std::wstring> newIntro);
    bool add_userInfo(userID_t id,std::shared_ptr<std::vector<std::wstring>> info );
    bool add_userFriend(userID_t id, std::shared_ptr<std::wstring> addedID);
    
    bool exist_email(std::shared_ptr<std::wstring>);
    bool exist_name(std::shared_ptr<std::wstring>);
    bool exist_id(uint id);
    
    bool remove_userFriend(userID_t id, std::shared_ptr<std::wstring> removedID);}
}
/*
            Design  Reference
        
    UserInfo data struct
        like this ( index by uint)
        {
            "name":     UTF8
            "password": UTF8     (only character but wstring)
            "id":       NUMBER 
            "reg-date": [y,m,d], (NUMBERs)
            "friends":  [UTF8, UTF8, ...] (UTF8ID)
            "intro": UTF8
            "email": UTF8 
        }

*/