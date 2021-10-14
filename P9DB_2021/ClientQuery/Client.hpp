#pragma once

#include "../../P9SDK/Interface/Interface.hpp"

#include "../Interpreter/Interface.hpp"

namespace P9
{

namespace Db
{

class client_config
{
public:
    string ip;
    string user_token;
};

class client
{
public:
    client(const client_config& config) :
        _config(config)
    {
    }

    ~client() = default;

    

private:
    client_config _config;
};

}
}
