#pragma once

#include "../../../Sdk/Interface.hpp"
#include "../../Algebra/Interface.hpp"

namespace gpx
{

struct dynamic_memory_desc
{
    string    _name;
    s64       _size;
};

class dynamic_memory
{
public:
    dynamic_memory() = default;

    virtual ~dynamic_memory() = default;

    virtual boole update(void* src) = 0;
};

}
