**Pavailion Nine SdkCore _Interface**
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
> <small>*define debug calibration level, including `DEBUG_FULL`, `DEBUG_CRIT`, `DEBUG_HALF`, `DEBUG_NONE`. see more detail in [SdkCore Debug Calibration](./SdkCore/Environment/DebugCalibration.hpp)*</small>

## Include _Interface Header

* Standard _Interface
> ```cpp
> #include "./_Interface.hpp"
> ```
> <small>*standard p9 sdk core interface, only P9 Sdk Core symbol being imported.*</small>

* Extensive _Interface
> ```cpp
> #include "./_InterfaceExt.hpp"
> ```
> <small>*extensive p9 sdk core interface, support advanced features:<br>*</small>
> * <small>*`C++ Coroutine Task`, see [P9 Sdk Core Task](./UnitTest/UT_Task.hpp).*</small>
