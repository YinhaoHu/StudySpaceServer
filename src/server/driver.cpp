//driver.cpp -- The driver of the server.
#define _SERVER_DRIVER

#include"../../lib/HData/HData.hpp"
#include"../../include/extern_onlineuser.hpp"

#ifndef HDATA_FOR_SERVER_LOCAL_en_USUTF8
#error "[lib]Make sure the ustf8 local set in HData & HWData construtor is en_utf8."
#endif

#include"server.hpp"
#include"guard.hpp"
#include"net.hpp"
#include"standard.hpp"

#include"../../lib/HThreadPool/HThreadPool.hpp"
#include"monitor.hpp"
#include<mutex>
#include<string>

#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<pthread.h>

#include<cwchar>
#include<cstdlib>
#include<cstring>

using namespace std;
using namespace ceh::Concurrency;

unsigned long long eventCount = 1;

static void initGlobalData();

int main(int argc, char* argv[])
{
    int listenfd, connfd;
    char clienthost[maxFieldSize], clientport[maxFieldSize];
    const char* vertifacation = "LIK";
    sockaddr_storage clientaddr;
    socklen_t clientaddrlen;
    HThreadPool threadPool(8);//Change the slots according to the needs.

    if(argc != 2)
    {
        guard::errorMsg(L"Usage :./server <port> ");
        exit(0);
    }

    if((listenfd = openListenfd(argv[1]) )< 0)
        guard::errorExit(L"[Error] main(): openListenfd");

    initGlobalData();
    std::thread T(monitor_entry);
    T.detach();

    while(true)
    {
        clientaddrlen = sizeof(sockaddr_storage);
        connfd = accept(listenfd, (sockaddr*)& clientaddr, &clientaddrlen);
        getnameinfo((sockaddr*)&clientaddr, clientaddrlen, clienthost, maxFieldSize, 
                                clientport, maxFieldSize, 0);

        send(connfd, vertifacation,4,0);
        //Inform the client that it is ok to connect with the server.

        threadPool.addTask(
            [](int socketfd)
            {
                serve(socketfd);
                close(socketfd);
            }, 
            connfd);
    }    
    return 0;
}

static void initGlobalData()
{
    userOfflineWaitingTasks = new std::unordered_map<userID_t, std::queue<std::shared_ptr<::wstring>>>;
    userOfflineWaitingTasks_MutexLock = new mutex;
    onlineUsers = new unordered_map<userID_t, userFD_t>;
    onlineUsersMutexLock = new mutex;
}