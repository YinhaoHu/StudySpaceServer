#include"guard.hpp"

#include<cstdio>
#include<cstdlib>
#include<cwchar>

using namespace std;


void guard::errorMsg(const wchar_t* msg)
{
    fwprintf(stderr, msg);
}
void guard::errorExit(const wchar_t* msg)
{
    fwprintf(stderr, msg);
    exit(0);
}