#pragma once

#include "../../Environment/Definition.h"
#include "../BuildinDefinition/BuildinType.h"
#include "../PlatformHeader.h"

#define ASSERT_EXCEPTION_CODE (0x09090909L)

#if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_LOG_ERROR

#define print(...) WindowsMsvcNs::printf(__VA_ARGS__)

#else

#define print(...)

#endif
