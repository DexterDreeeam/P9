#pragma once

#if defined(I_AM_LINUX_GCC)

    #include "EnvironmentLinuxGcc/_Definition.hpp"

#elif defined(I_AM_WINDOWS_MSVC)

    #include "EnvironmentWindowsMsvc/_Definition.hpp"

#else

    #error "Not supported environment"

#endif
