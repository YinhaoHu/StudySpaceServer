#include"serverdatabase.hpp"
#include"../server/standard.hpp"
#include"../server/guard.hpp"


using namespace guard;
using namespace std;
using json = nlohmann::json;


ServerDataBase::ServerDataBase()
{
    in = new ifstream(standard::filesystem::serverDataBase_Name);

    try
    {
        j = new json(json::parse(*in));
    }
    catch(json::parse_error& e)
    {
        errorExit(L"ServerDataBase: json-parse_error.");
    }
}


ServerDataBase::~ServerDataBase()
{
    ofstream * out = new ofstream(standard::filesystem::serverDataBase_Name);

    (*out) << setw(4) << (*j);

    out->close();
    in->close();
    delete j;
    delete in;
}

std::shared_ptr<std::wstring> ServerDataBase::getRegisterCode()const
{
    auto code_num_item = (*j)["register-code"];
    if(code_num_item.is_null())
    {
        errorExit(L"Server data miss: register-code");
    }
    
    uint code_num = code_num_item.get<uint>();
    monitor(L"Track -> ServerDataBase: getRegisterCode");
    return make_shared<wstring>(to_wstring(code_num));
}
