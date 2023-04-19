#pragma once

#include<mutex>
#include<queue>
#include<functional>
#include<unordered_map>
#include<string>
#include<memory>

#include"../src/service/service.hpp"

#ifndef _SERVER_DRIVER
extern 
#endif
std::unordered_map<userID_t, std::queue<std::shared_ptr<std::wstring>>>
                *userOfflineWaitingTasks;

#ifndef _SERVER_DRIVER
extern 
#endif
std::mutex* userOfflineWaitingTasks_MutexLock;

#ifndef _SERVER_DRIVER
extern 
#endif
std::unordered_map<userID_t, userFD_t>* onlineUsers ;

#ifndef _SERVER_DRIVER
extern 
#endif
std::mutex* onlineUsersMutexLock;