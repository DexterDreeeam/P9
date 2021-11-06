#pragma once

namespace _InternalNs
{

template<typename ...Args>
void p9_print(const char* const format, Args ...args);

}

#if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_LOG_ERROR

#define print(...) _InternalNs::p9_print(__VA_ARGS__)

#else

#define print(...)

#endif
