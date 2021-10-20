#pragma once

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

#define DEBUG_LEVEL_CALIBRATION_ALL               95

#define DEBUG_LEVEL_CALIBRATION_LOG_TIMER         85
#define DEBUG_LEVEL_CALIBRATION_LOG_CALLSTACK     75
#define DEBUG_LEVEL_CALIBRATION_LOG_NORM          65
#define DEBUG_LEVEL_CALIBRATION_LOG_ERROR         55
#define DEBUG_LEVEL_CALIBRATION_LOG_NONE          45

#define DEBUG_LEVEL_CALIBRATION_CHECK_ALLPARAM    35
#define DEBUG_LEVEL_CALIBRATION_CHECK_ALLINPUT    25
#define DEBUG_LEVEL_CALIBRATION_CHECK_BASIC       15

#define DEBUG_LEVEL_CALIBRATION_RUNTIME            5


#if   DEBUG_CALIBRATION == DEBUG_FULL
    #define DEBUG_LEVEL DEBUG_LEVEL_CALIBRATION_ALL
#elif DEBUG_CALIBRATION == DEBUG_CRIT
    #define DEBUG_LEVEL DEBUG_LEVEL_CALIBRATION_LOG_NORM
#elif DEBUG_CALIBRATION == DEBUG_HALF
    #define DEBUG_LEVEL DEBUG_LEVEL_CALIBRATION_LOG_ERROR
#elif DEBUG_CALIBRATION == DEBUG_NONE
    #define DEBUG_LEVEL DEBUG_LEVEL_CALIBRATION_RUNTIME
#else
    #error: Not supported DEBUG_LEVEL.
#endif
