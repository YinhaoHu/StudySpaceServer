#pragma once

#include<memory>
#include<string>
#include"../../include/extern_onlineuser.hpp"

void noticeUser(userID_t id, std::shared_ptr<std::wstring> msg);

void doGetNotification(serviceInfo* info);