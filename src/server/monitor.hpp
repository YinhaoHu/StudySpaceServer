#pragma once

#include"../data/userdatabase.hpp"
#include"../../lib/Json/json.hpp"

class Monitor
{
private:
    nlohmann::json *j;

public:
    Monitor();
    ~Monitor();

    void disPlay_UserNameSet();
    void disPlay_OnlineUsers();
};


void monitor_entry();