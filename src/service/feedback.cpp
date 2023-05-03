#include"feedback.hpp"
#include"../server/standard.hpp"
#include"../../lib/HString/HString.hpp"
#include"../server/net.hpp"

#include<string>
#include<fstream>
#include<cstring>

#include<sys/stat.h>

using namespace std;
using namespace ceh::String;

void doGetFeedBack(serviceInfo* info)
{
    auto data = easyRecv_WString(info->userfd);

    char* idstr = new char[16];
    memset(idstr, 0, 16);
    uintToStr(info->userid, idstr);

    char* filename = new char[64];
    memset(filename, 0, 64);
    strcpy(filename,standard::feedBackDirName);
    strcat(filename, idstr);

    wofstream* out = new wofstream(filename, ios::app);
    wstring* newline = new wstring(L"\n\n\t");

    out->imbue(locale("en_US.UTF-8"));
    out->write(newline->c_str(), newline->length());
    out->write(data->c_str(), data->length());
    out->close();

    delete idstr;
    delete newline;
    delete filename;
    delete out;
}