#pragma once

namespace _InternalNs
{

void _C_CALL_ p9_print(const char* const format, ...);

}

#if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_LOG_ERROR

#define print(...) _InternalNs::p9_print(__VA_ARGS__)

#else

#define print(...)

#endif
