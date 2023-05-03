#pragma once

#include"../../lib/Json/json.hpp"

#include<fstream>


class ServerDataBase
{
private:
    nlohmann::json* j;
    std::ifstream *in;

public:
    ServerDataBase();
    ~ServerDataBase();

    std::shared_ptr<std::wstring> getRegisterCode()const;
};