#pragma once

/*
    Support many easy methods to access the userdata
    using only id.
*/
#include"service.hpp"

#include<memory>
#include<string>
#include<vector>

namespace serverData
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

    bool init_userfriend(userID_t id);

    bool check_userExist(userID_t id);

    std::shared_ptr<std::wstring> get_userID(userID_t id);
    std::shared_ptr<std::wstring> get_userIntro(userID_t id);
    std::shared_ptr<std::wstring> get_userName(userID_t id);
    std::shared_ptr<std::wstring> get_userEMail(userID_t id);
    std::shared_ptr<std::wstring> get_userPassword(userID_t id);
    std::shared_ptr<DataNode> get_userProfile(userID_t id); 
    std::shared_ptr<std::vector<std::wstring>> get_userFriends(userID_t id);

    bool modify_userIntro(userID_t id,std::shared_ptr<std::wstring> newIntro);
    bool modify_userName(userID_t id,std::shared_ptr<std::wstring> newUsername);
    bool modify_userEMail(userID_t id,std::shared_ptr<std::wstring> newEMail);
    bool modify_userPassword(userID_t id, std::shared_ptr<std::wstring> newPassword);
    bool modify_userProfile(userID_t id, std::shared_ptr<char> newProfile, size_t filesize); 

    bool add_userIntro(userID_t id,std::shared_ptr<std::wstring> newIntro);
    bool add_userInfo(userID_t id,std::shared_ptr<std::vector<std::wstring>> info );
    bool add_userFriend(userID_t id, std::shared_ptr<std::wstring> addedID);
    
    bool remove_userFriend(userID_t id, std::shared_ptr<std::wstring> removedID);
}