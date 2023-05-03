#include"convert.hpp"
#include<locale>
#include<codecvt>
#include<algorithm>

using namespace std;

 string wstring_to_utf8(wstring& wide_string)
{
     wstring_convert<codecvt_utf8<wchar_t>> utf8_conv;
    return utf8_conv.to_bytes(wide_string);
}

 wstring utf8_to_wstring(string& utf8_string)
{
    wstring_convert<codecvt_utf8<wchar_t>> utf8_conv;
    return utf8_conv.from_bytes(utf8_string);
}

  string wstring_to_utf8(wstring&& wide_string)
{
    wstring_convert<codecvt_utf8<wchar_t>> utf8_conv;
    return utf8_conv.to_bytes(wide_string);
}

  wstring utf8_to_wstring(string&& utf8_string)
{
    wstring_convert<codecvt_utf8<wchar_t>> utf8_conv;
    return utf8_conv.from_bytes(utf8_string);
}