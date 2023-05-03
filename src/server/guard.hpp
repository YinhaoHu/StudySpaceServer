#ifndef _GUARD_HPP
#define _GUARD_HPP

#include<iostream>
#include<string>

#include"../../lib/HTime/HTime.hpp"

namespace guard
{
    enum MonitorType{Send, Recv, Done};

    void errorExit(const wchar_t* msg);
    void errorMsg(const wchar_t* msg);

    void monitor(const wchar_t* msg);
    void monitor(const wchar_t* type, const wchar_t* msg);
}





#endif