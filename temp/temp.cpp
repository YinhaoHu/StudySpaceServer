#include<cstring>
#include<cstdio>

static void breakPoint()
{
    printf("Break Point is here.\n");
    getchar();
}
class MemoryControl
{
private:
    char staticData[1000000];
public:
    MemoryControl()
    {
        memset(staticData,1,4096);
    }
    ~MemoryControl(){}
};

int main()
{
    {
        MemoryControl mc;
        breakPoint();
    }
    breakPoint();
    return 0;
}