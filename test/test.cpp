#include<cstdio>
#include<string>
#include<algorithm>
#include<vector>
#include<iostream>

using namespace std;

template<typename... Args>
void multiPrint(Args... args)
{
    std::cout<< args,0;
}

int main()
{
    multiPrint(1,5,6);

    return 0;
}