#pragma once

#include "../../P9SDK/Interface/Interface.hpp"
#include "../Interpreter/Interface.hpp"
#include "QueryCommand.hpp"

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

    list<ref<json_base>> execute_query();

private:
    client_config _config;
};

}
}
