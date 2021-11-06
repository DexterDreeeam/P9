
#include "../../Interface.hpp"
#include "../EnvironmentHeader.hpp"

namespace _InternalNs
{

template<typename ...Args>
void p9_print(const char* const format, Args ...args)
{
    printf(format, args...);
}

}
