#include"serverdata.hpp"
#include"serverdatabase.hpp"

using namespace data::server;
using namespace std;


shared_ptr<wstring> data::server::get_registerCode()
{
    auto sdb = new ServerDataBase;

    auto res = sdb->getRegisterCode();
    delete sdb;
    return res;
}

