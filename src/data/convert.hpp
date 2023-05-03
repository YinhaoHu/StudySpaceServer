#include<string>

  std::string wstring_to_utf8(std::wstring& wide_string);
  std::string wstring_to_utf8(std::wstring&& wide_string);
  std::wstring utf8_to_wstring(std::string& utf8_string);
  std::wstring utf8_to_wstring(std::string&& utf8_string);