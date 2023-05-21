#include"monitor.hpp"

#include"../../include/extern_onlineuser.hpp"
#include<string>
#include<iostream>

using namespace std;
using json = nlohmann::json;

static void list_name();
static void list_online();
static void help();

static vector<pair<string, function<void()>>> cmds{
    {"list-name"        ,   list_name},
    {"list-online"      ,   list_online},
    {"help"             ,   help}
};

static inline void beginPrint(const char* msg);
static inline void endPrint();

void monitor_entry()
{ 
    string* input = new string;

    beginPrint("SERVER MONITOR");
    printf(">");
    while( cin >> (*input))
    {
        auto found_iter =  find_if(cmds.begin(), cmds.end(), 
            [input](pair<string,function<void()>>& cmd )
            {
                return input->compare(cmd.first) == 0;
            });

        if(found_iter != cmds.end())
            found_iter->second();
        else 
            printf("Invalid commond. Enter help for more information.\n"); 

        input->clear();
        printf("> ");
    }
}


static inline void beginPrint(const char* msg)
{
    printf("\t\t----------%20s-----------\n", msg);
}

static inline void endPrint()
{

    printf("\t\t----------------------------------------\n\n");
}

static void list_name()
{
    Monitor m;
    m.disPlay_UserNameSet();
}

static void list_online()
{
    Monitor m;
    m.disPlay_OnlineUsers();
}

static void help()
{
    beginPrint("SUPPORTED COMMANDS");
    uint index = 0;
    for(auto cmd : cmds)
        printf("%6d %s\n", index++, cmd.first.c_str());
    endPrint();
}

// Class Implementation
Monitor::Monitor()
{
    UserDataBase udb;
    auto oldj = udb.getRawData();
    j = new json(oldj);
}

Monitor::~Monitor()
{
    delete j;
}

void Monitor::disPlay_UserNameSet()
{
    beginPrint("USER NAME LIST");
    uint cnt = 0;
    for(auto& item : *j)
    {
        auto s = item["name"].get<string>();
        printf("%12d %s\n", cnt++,s.c_str());
    }
    endPrint();
}

void Monitor::disPlay_OnlineUsers()
{
    beginPrint("ONLINE USER LIST");
    unique_lock lock(*onlineUsersMutexLock);
    uint cnt = 0;
    for(auto& id_fd : *onlineUsers)
    {
        printf("%12d %12lld\n", cnt++, id_fd.first);
    }
    endPrint();
}
