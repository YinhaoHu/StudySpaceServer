#ifndef _NET_HPP
#define _NET_CPP

#include<sys/socket.h>
#include<memory>
#include<string>

struct NetFileNode{
    size_t bytes;
    std::shared_ptr<char> data;

    NetFileNode(size_t _bytes = 0, char* _data = nullptr):
        bytes(_bytes), data(std::shared_ptr<char>(_data))
        {}
    ~NetFileNode(){}
};

int openListenfd(char* port);

std::shared_ptr<NetFileNode> easyRecv_File(int sockfd);

std::shared_ptr<std::wstring> easyRecv_WString(int sockfd);

void easySend_WString(int sockfd, std::shared_ptr<std::wstring> string);

void easySendMsg(int sockfd, std::shared_ptr<std::wstring> string);

void easySend_File(int sockfd, std::shared_ptr<char> fileData, size_t fileSize);


#endif