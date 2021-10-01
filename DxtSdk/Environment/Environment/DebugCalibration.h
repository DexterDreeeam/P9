#pragma once

#include "Definition.h"

#define DEBUG_FULL 3  //# all logs and all checkers enabled
#define DEBUG_CRIT 2  //# critical logs and all checkers enabled
#define DEBUG_HALF 1  //# no log and all checkers enabled
#define DEBUG_NONE 0  //# no log and no checker enabled

/*
 *  Debug Level
 *    - DEBUG_FULL
 *        function call timer
 *
 *    - DEBUG_CRIT
 *        function call stack
 *        normal log
 *        error log
 *
 *    - DEBUG_HALF
 *        inside checker
 *        input checker
 *
 *    - DEBUG_NONE
 */

//======================================

#if   DEBUG_CALIBRATION == DEBUG_FULL
    #define DEBUG_LEVEL 100
#elif DEBUG_CALIBRATION == DEBUG_CRIT
    #define DEBUG_LEVEL 80
#elif DEBUG_CALIBRATION == DEBUG_HALF
    #define DEBUG_LEVEL 50
#elif DEBUG_CALIBRATION == DEBUG_NONE
    #define DEBUG_LEVEL 0
#else
    #error: dxt_error: don't supported DEBUG_LEVEL.
#endif

#define DEBUG_LEVEL_CALIBRATION_ALL               95
#define DEBUG_LEVEL_CALIBRATION_LOG_TIMER         95
#define DEBUG_LEVEL_CALIBRATION_LOG_CALLSTACK     85
#define DEBUG_LEVEL_CALIBRATION_LOG_NORM          75
#define DEBUG_LEVEL_CALIBRATION_LOG_ERROR         65
#define DEBUG_LEVEL_CALIBRATION_LOG_NONE          55
#define DEBUG_LEVEL_CALIBRATION_CHECK_ALLPARAM    45
#define DEBUG_LEVEL_CALIBRATION_CHECK_ALLINPUT    35
#define DEBUG_LEVEL_CALIBRATION_CHECK_BASIC       25
#define DEBUG_LEVEL_CALIBRATION_NONE               5
