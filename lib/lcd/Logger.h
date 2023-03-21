#pragma once
#include "Print.h"


#define C_LOG_NONE      0
#define C_LOG_FATAL     1
#define C_LOG_ALERT     2
#define C_LOG_CRIT      3
#define C_LOG_ERROR     4
#define C_LOG_WARNING   5
#define C_LOG_NOTICE    6
#define C_LOG_INFO      7
#define C_LOG_DEBUG     8

#ifndef C_ENABLE_LOG_LEVEL
#define C_ENABLE_LOG_LEVEL  C_LOG_DEBUG
#endif
#define c_logf(args...) c_printfln(args)

#if C_ENABLE_LOG_LEVEL >= C_LOG_DEBUG
#define c_debug(args...) c_logf("D:" args)
#else
#define c_debug(args...)
#endif

#if C_ENABLE_LOG_LEVEL >= C_LOG_INFO
#define c_info(args...) c_logf("I:" args)
#else
#define c_info(args...)
#endif

#if C_ENABLE_LOG_LEVEL >= C_LOG_NOTICE
#define c_notice(args...) c_logf("N:" args)
#else
#define c_notice(args...)
#endif

#if C_ENABLE_LOG_LEVEL >= C_LOG_WARNING
#define c_warning(args...) c_logf("W:" args)
#else
#define c_warning(args...)
#endif

#if C_ENABLE_LOG_LEVEL >= C_LOG_ERROR
#define c_error(args...) c_printf("E:" args)
#else
#define c_error(args...)
#endif

#if C_ENABLE_LOG_LEVEL >= C_LOG_CRIT
#define c_crit(args...) c_logf("C:" args)
#else
#define c_crit(args...)
#endif

#if C_ENABLE_LOG_LEVEL >= C_LOG_ALERT
#define c_alert(args...) c_logf("A:" args)
#else
#define c_alert(args...)
#endif

#if C_ENABLE_LOG_LEVEL >= C_LOG_FATAL
#define c_fatal(args...) c_logf("F:" args)
#else
#define c_fatal(args...)
#endif

