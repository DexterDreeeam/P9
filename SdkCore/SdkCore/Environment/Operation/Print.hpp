#pragma once

namespace _Internal {
namespace _Print {

void _C_CALL_ p9_print(const char* const format, ...);

}} // _Internal::_Print

#if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_LOG_ERROR

#define print(...) _Internal::_Print::p9_print(__VA_ARGS__)

#else

#define print(...)

#endif
