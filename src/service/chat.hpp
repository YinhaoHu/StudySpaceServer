#pragma once

#include"service.hpp"

//forward msg
void do_privateChatSend(serviceInfo* info);

//send to the Client which requrie msg.
void do_privateChatRecv(serviceInfo* info);