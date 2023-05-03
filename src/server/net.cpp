#include"net.hpp"
#include"guard.hpp"
#include"standard.hpp"
#include"../../lib/HTime/HTime.hpp"
#include"../../lib/HString/HString.hpp"

#include<netdb.h>
#include<sys/socket.h>
#include<unistd.h>
#include<iostream>
#include<cwchar>
#include<cstring>
#include<cstdio>

using namespace std;
using namespace ceh::String;

const int listenLimit = 8192;
const size_t sizeInfoBytes = 16;

int openListenfd(char* port)
{
    int listenfd, optval = 1;
    addrinfo hints, *result, *p;

    memset((void*)&hints, 0, sizeof(addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_ADDRCONFIG | AI_NUMERICSERV | AI_PASSIVE;
    if(getaddrinfo(nullptr, port, &hints, &result))
    {    
        guard::errorExit(L"[Error] openListenfd: getaddrinfo() - ");
    }

    for(p = result; p != nullptr; p= p->ai_next)
    {
        if((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue;
        
         setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,(const void*)&optval, sizeof(int));// ?

         if(bind(listenfd, p->ai_addr, p->ai_addrlen) ==  0)
            break;
        close(listenfd);
    }
    freeaddrinfo(result);

    if(p == nullptr)
        return -1;
    if(listen(listenfd, listenLimit) < 0)
    {    
        close(listenfd);
        return -1;
    }
    return listenfd;
}

std::shared_ptr<NetFileNode> easyRecv_File(int sockfd)
{
    size_t fileSize;
    char* fileSizeStr;
    char* fileData; //It will be delete automatically.

    fileSizeStr = new char[sizeInfoBytes];
    memset(fileSizeStr, 0, sizeInfoBytes);
    recv(sockfd, fileSizeStr, sizeInfoBytes, MSG_WAITALL);
    fileSize = atol(fileSizeStr);

    fileData = new char[fileSize];
    recv(sockfd, fileData, fileSize, MSG_WAITALL);
    guard::monitor(L"Track -> easyRecv_WString",to_wstring(fileSize).c_str());
    delete[] fileSizeStr;
    return make_shared<NetFileNode>(fileSize, fileData);
}

std::shared_ptr<wstring> easyRecv_WString(int sockfd)
{
    size_t bytes;
    char* sizeStr;
    wchar_t* wcstr;

    sizeStr = new char[sizeInfoBytes];
    memset(sizeStr, 0, sizeInfoBytes);
    recv(sockfd, sizeStr, sizeInfoBytes, MSG_WAITALL);

    bytes = atol(sizeStr);
    wcstr = new wchar_t[bytes / sizeof(wchar_t)];
    memset(wcstr, 0, bytes);
    recv(sockfd, wcstr, bytes, MSG_WAITALL);

    auto res = make_shared<wstring>(wcstr);
    res->resize(bytes / sizeof(wchar_t));

    guard::monitor(L"Track -> easyRecv_WString",res->c_str());

    delete[] sizeStr;
    delete[] wcstr;
    return res;
}

void easySend_WString(int sockfd, std::shared_ptr<wstring> string)
{
    char* sizeStr = new char[sizeInfoBytes];
    size_t bytes = string->length() * sizeof(wchar_t);
    wchar_t* wcstr = new wchar_t[bytes / sizeof(wchar_t)];

    memset(sizeStr, 0, sizeInfoBytes);
    uintToStr(bytes, sizeStr);

    memset(wcstr, 0, bytes);
    memcpy(wcstr, string->c_str(), bytes);
    send(sockfd, sizeStr, sizeInfoBytes, 0);
    send(sockfd, wcstr, bytes, 0);
    guard::monitor(L"Track -> easySend_Wstring", string->c_str());
    delete[] wcstr;
    delete[] sizeStr;
}

void easySend_File(int sockfd, std::shared_ptr<char> fileData, size_t fileSize)
{
    char* sizeStr = new char[sizeInfoBytes];

    memset(sizeStr, 0, sizeInfoBytes);
    uintToStr(fileSize, sizeStr);

    send(sockfd, sizeStr, sizeInfoBytes, 0);
    send(sockfd, fileData.get(), fileSize,0);
    guard::monitor(L"Track -> easySend_File", to_wstring(fileSize).c_str());
}

void easySendMsg(int sockfd, std::shared_ptr<wstring> string)
{
    wchar_t* wcstr = new wchar_t[maxSockBufferSize];
    memset(wcstr,0,maxSockBufferBytes);
    memcpy(wcstr, string->c_str(), string->length() * sizeof(wchar_t));
    send(sockfd, string->c_str(), maxSockBufferBytes, 0);
    guard::monitor(L"Track -> easySend_Msg", string->c_str());
    delete wcstr;
}
