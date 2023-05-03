#pragma once

#include<memory>
#include<string>

namespace data
{
    namespace server{
        std::shared_ptr<std::wstring> get_registerCode();
    }
}