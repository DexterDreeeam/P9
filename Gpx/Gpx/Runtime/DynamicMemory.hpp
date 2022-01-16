#pragma once

#include "../../../Sdk/Interface.hpp"
#include "../../Algebra/Interface.hpp"

namespace gpx
{

struct dynamic_memory_desc
{
    string    name;
    s64       size;
};

class dynamic_memory
{
public:
    dynamic_memory() = default;

    virtual ~dynamic_memory() = default;

    virtual s64 memory_size() = 0;

    virtual boole update(void* src) = 0;
};

}
