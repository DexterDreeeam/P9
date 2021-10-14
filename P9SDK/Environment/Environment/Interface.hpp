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
#elif WHO_I_AM == I_AM_LINUX_GCC
    #include "../Environment_Linux_Gcc/Interface.hpp"
#endif
