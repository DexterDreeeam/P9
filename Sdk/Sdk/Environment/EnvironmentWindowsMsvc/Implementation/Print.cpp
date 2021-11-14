
#include "../EnvironmentHeader.hpp"
#include "../../Interface.hpp"

namespace _InternalNs
{

void _C_CALL_ p9_print(const char* const format, ...)
{
    va_list va_l;
    va_start(va_l, format);
    vprintf(format, va_l);
    va_end(va_l);
}

}
