#pragma once

#define NsBegin(MyNs)       namespace ns {
#define NsEnd(MyNs)         }

#define IntNsBegin(MyNs)    NsBegin(_Internal) NsBegin(MyNs)
#define IntNsEnd(MyNs)      }} // _Internal::MyNs namespace end
#define IntNs(MyNs)         _Internal::MyNs

#define Ns(x)               x::

#if defined(I_AM_LINUX_GCC)
    #include "EnvironmentLinuxGcc/_Definition.hpp"
#elif defined(I_AM_WINDOWS_MSVC)
    #include "EnvironmentWindowsMsvc/_Definition.hpp"
#else
    #error "Not supported environment"
#endif
