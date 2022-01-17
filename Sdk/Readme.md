**Pavailion Nine Sdk Interface**
=========

## Precompile Macro

* Setup Build Environment
> ```cpp
> #define I_AM_WINDOWS_MSVC
> ```
> <small>*define build environment, including `I_AM_WINDOWS_MSVC`, `I_AM_LINUX_GCC`.*</small>

* Setup Application Name
> ```cpp
> #define SERVICE_NAME "TestApplication"
> ```
> <small>*define application name, this string would be used in global application terminator.*</small>

* Setup Debug Calibration
> ```cpp
> #define DEBUG_CALIBRATION DEBUG_FULL
> ```
> <small>*define debug calibration level, including `DEBUG_FULL`, `DEBUG_CRIT`, `DEBUG_HALF`, `DEBUG_NONE`. see more detail in [Sdk Debug Calibration](./Sdk/Environment/DebugCalibration.hpp)*</small>

## Include Interface Header

* Standard Interface
> ```cpp
> #include "./Interface.hpp"
> ```
> <small>*standard p9 sdk interface, only P9 Sdk symbol being imported.*</small>

* Extensive Interface
> ```cpp
> #include "./InterfaceExt.hpp"
> ```
> <small>*extensive p9 sdk interface, support advanced features:<br>*</small>
> * <small>*`C++ Coroutine Task`, see [P9 Sdk Task](./EntryPoint/UT_Task.hpp).*</small>
