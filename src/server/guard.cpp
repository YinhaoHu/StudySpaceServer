#include"guard.hpp"
#include"../data/convert.hpp"

#include<cstdio>
#include<cstdlib>
#include<cwchar>
#include<iostream>
#include<iomanip>
#include<fstream>

uint logID = 0;

using namespace std;

static FILE* out_log = fopen("data/log.out", "w");
static FILE* out_err = fopen("data/err.out", "w");

void guard::errorMsg(const wchar_t* msg)
{
    fprintf(out_err, "%s\n", wstring_to_utf8(msg).c_str()); 
}
void guard::errorExit(const wchar_t* msg)
{
    fprintf(out_err , "%s %s\n", wstring_to_utf8(msg).c_str(), "[Server End]");
    fprintf(stdout, "Server End, check err.out for more information. \n\n") ;
    exit(0);
}

void guard::monitor(const wchar_t* msg)
{
    guard::monitor(L"Minitor", msg);
}

void guard::monitor(const wchar_t* type, const wchar_t* msg)
{
    ceh::Time::HTime htime;
    
    fprintf(out_log, "[%8d][%2d %2d %2d] %s \n\t %s\n\n", logID++,
    htime.getHourByInt()+8,htime.getMinByInt(),htime.getSecByInt(),
    wstring_to_utf8(type).c_str(),
    wstring_to_utf8(msg).c_str()
    );
}
