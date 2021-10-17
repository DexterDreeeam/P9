/* ********* * ********* * ********* * ********* * *********
 *
 * Environment/Interface.h
 *
 * proj: Environment
 * desc: Project Interface
 * autr: dexterdreeeam
 * date: 20211001
 * tips:
 *
 ********* * ********* * ********* * ********* * ********* */

#pragma once

#include "Definition.hpp"

#if   WHO_I_AM == I_AM_WINDOWS_MSVC

    #include "../Environment_Windows_Msvc/Interface.hpp"
    #define P9_FOLDER   WINDOWS_MSVC_LOG_FOLDER

#elif WHO_I_AM == I_AM_LINUX_GCC

    #include "../Environment_Linux_Gcc/Interface.hpp"
    #define P9_FOLDER   LINUX_GCC_LOG_FOLDER

#endif

#define LOG_FOLDER     P9_FOLDER "LOG/"
