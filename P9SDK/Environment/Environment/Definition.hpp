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

#include "../../../EnvironmentSetting.inl"

#if !defined(WHO_I_AM) || !defined(SERVICE_NAME) || !defined(DEBUG_CALIBRATION)

    // should be set up in ~/EnvironmentSetting.inl

    // I_AM_WINDOWS_MSVC
    // I_AM_LINUX_GCC
    #define \
        WHO_I_AM \
            I_AM_WINDOWS_MSVC

    #define \
        SERVICE_NAME \
            "Unname"

    #define \
        DEBUG_CALIBRATION \
            DEBUG_FULL

#endif

#include "DebugCalibration.hpp"
