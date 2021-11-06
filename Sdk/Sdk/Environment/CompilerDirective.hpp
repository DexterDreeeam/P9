#pragma once

#if defined(I_AM_LINUX_GCC)

    #include "EnvironmentLinuxGcc/CompilerDirective.hpp"

#elif defined(I_AM_WINDOWS_MSVC)

    #include "EnvironmentWindowsMsvc/CompilerDirective.hpp"

#else

    #error "Not supported environment"

#endif
