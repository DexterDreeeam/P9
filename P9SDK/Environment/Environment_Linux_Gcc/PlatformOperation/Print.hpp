#pragma once

#include "../../Environment/Definition.hpp"
#include "../BuildinDefinition/BuildinType.hpp"
#include "../PlatformHeader.hpp"

#define ASSERT_EXCEPTION_CODE (0x09090909L)

#if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_LOG_ERROR

#define print(...) LinuxGccNs::printf(__VA_ARGS__)

#else

#define print(...)

#endif
