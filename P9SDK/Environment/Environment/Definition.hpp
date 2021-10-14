/* ********* * ********* * ********* * ********* * *********
 *
 * Environment/Environment/Definition.h
 *
 * proj: Environment
 * desc: precompile definitions
 * autr: dexterdreeeam
 * date: 20211001
 * tips:
 *
 ********* * ********* * ********* * ********* * ********* */

#pragma once

#include "../Environment_Windows_Msvc/Definition.hpp"
#include "../Environment_Linux_Gcc/Definition.hpp"
#include "DebugCalibrationDefinition.hpp"

// I_AM_WINDOWS_MSVC
// I_AM_LINUX_GCC
#define \
    WHO_I_AM \
        I_AM_LINUX_GCC

#define \
    DEBUG_CALIBRATION \
        DEBUG_FULL

#include "DebugCalibration.hpp"
