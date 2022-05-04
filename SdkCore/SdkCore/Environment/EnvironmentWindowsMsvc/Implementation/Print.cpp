
#include "../../_Interface.hpp"
#include "../EnvironmentHeader.hpp"

namespace _Internal {
namespace _Print {

void _C_CALL_ p9_print(const char* const format, ...)
{
    va_list va_l;
    va_start(va_l, format);
    vprintf(format, va_l);
    va_end(va_l);
}

}} // _Internal::_Print
