//==============================================================================
//
// Copyright (c) 2023 Qualcomm Technologies, Inc.
// All Rights Reserved.
// Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//==============================================================================

#ifndef LOG_H
#define LOG_H 1

#include "weak_linkage.h"
#include "macros_attribute.h"
#include <cstdarg>
#include <string>
#include <chrono>

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __FUNC_INFO__ __FUNCSIG__
#else
#define __FUNC_INFO__ __PRETTY_FUNCTION__
#endif

// GCC and Clang define a preprocessor macro which is just the basename of the current file.
#if defined(__FILE_NAME__)
#define FILE_BASENAME __FILE_NAME__
#else

// MSVC doesn't have this nice feature, so we have to do it manually.  Note that the entire path
// still ends up in the .rodata section, unfortunately.

// Constexpr that will strip the path off of the file for logging purposes
constexpr char const *stripFilePath(const char *path)
{
    const char *file = path;
    while (*path) {
        if (*path++ == '/') {
            file = path;
        }
    }
    return file;
}

#define FILE_BASENAME stripFilePath(__FILE__)

#endif // defined(__FILE_NAME__)

#define STRINGIZE_DETAIL(X) #X
#define STRINGIZE(X)        STRINGIZE_DETAIL(X)

#include "graph_status.h"
#include "cc_pp.h"

#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif

// If log level or the dynamic logging flag are defined but don't have a value,
// then consider them to be undefined.
#if ~(~NN_LOG_MAXLVL + 0) == 0 && ~(~NN_LOG_MAXLVL + 1) == 1
#undef NN_LOG_MAXLVL
#endif

#if ~(~NN_LOG_DYNLVL + 0) == 0 && ~(~NN_LOG_DYNLVL + 1) == 1
#undef NN_LOG_DYNLVL
#endif

/*
 * We have migrated using C++ features like iostream to printf strings.
 * Why?
 * * C++ iostream makes it more difficult to use mixed decimal/hex
 * * C++ iostream isn't easily compatible with on-target logging facilities
 * * C++ iostream is bad for code size, printf is much better
 */

//Log levels macro
#define NN_LOG_ERRORLVL         0 //Error log level is 0
#define NN_LOG_WARNLVL          1 //Warning log level is 1
#define NN_LOG_STATLVL          2 //Stats log level is 2
#define NN_LOG_INFOLVL          3 //Info log level is 3
#define NN_LOG_VERBOSELVL       4 //Verbose log level is from 4-10
#define NN_LOG_STATLVL_INTERNAL 8
#define NN_LOG_INFOLVL_INTERNAL 9
#define NN_LOG_DEBUGLVL         11 //Debug log level is > 10

typedef void (*DspLogCallbackFunc)(int level, const char *fmt, va_list args);

// Dynamically set the logging priority level.
PUSH_VISIBILITY(default)
EXTERN_C_BEGIN
extern "C" {

API_FUNC_EXPORT void SetLogPriorityLevel(int level);
API_FUNC_EXPORT int GetLogPriorityLevel();
API_FUNC_EXPORT void SetLogCallbackFunc(DspLogCallbackFunc fn);
API_FUNC_EXPORT DspLogCallbackFunc GetLogCallbackFunc();

// This prevents preemption if we're using the TID preemption mechanism.
// Enable format checking when we're ready to fix all of the broken formats!
// [[gnu::format(printf, 1, 2)]]
API_FUNC_EXPORT void nn_log_printf(const char *fmt, ...);
}
EXTERN_C_END
POP_VISIBILITY()

#ifdef __cplusplus
extern "C" {
#endif

// special log message for x86 that will log regardless logging level
void qnndsp_x86_log(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

/////////////////////////ENABLE_QNN_LOG
#ifdef ENABLE_QNNDSP_LOG

PUSH_VISIBILITY(default)
#include "weak_linkage.h"

API_FUNC_EXPORT API_C_FUNC void API_FUNC_NAME(SetLogCallback)(DspLogCallbackFunc cbFn, int logPriority);

extern "C" {
API_FUNC_EXPORT void qnndsp_log(int prio, const char *FMT, ...);

API_FUNC_EXPORT void hv3_load_log_functions(decltype(SetLogCallback) **SetLogCallback_f);
}
POP_VISIBILITY()

#define qnndsp_base_log(prio, cformat, ...) (void)(qnndsp_log(prio, cformat, ##__VA_ARGS__))

#define rawlog(cformat, ...) (qnndsp_base_log(NN_LOG_ERRORLVL, cformat, ##__VA_ARGS__), GraphStatus::ErrorFatal)
#define okaylog(cformat, ...)                                                                                          \
    (qnndsp_base_log(NN_LOG_ERRORLVL, "%s:" STRINGIZE(__LINE__) ":" cformat "\n", FILE_BASENAME, ##__VA_ARGS__),       \
     GraphStatus::ErrorFatal)
#define errlog(cformat, ...)                                                                                           \
    (qnndsp_base_log(NN_LOG_ERRORLVL, "%s:" STRINGIZE(__LINE__) ":ERROR:" cformat "\n", FILE_BASENAME, ##__VA_ARGS__), \
     GraphStatus::ErrorFatal)
#define warnlog(cformat, ...)        qnndsp_base_log(NN_LOG_WARNLVL, "WARNING: " cformat "\n", ##__VA_ARGS__)
#define statlog(statname, statvalue) qnndsp_base_log(NN_LOG_STATLVL, "STAT: %s=%lld\n", statname, (long long)statvalue)
#define i_statlog(statname, statvalue)                                                                                 \
    qnndsp_base_log(NN_LOG_STATLVL_INTERNAL, "STAT: %s=%lld\n", statname, (long long)statvalue)
#define statslog(statname, statvalue)   qnndsp_base_log(NN_LOG_STATLVL, "STAT: %s=%s\n", statname, statvalue)
#define i_statslog(statname, statvalue) qnndsp_base_log(NN_LOG_STATLVL_INTERNAL, "STAT: %s=%s\n", statname, statvalue)
#define infolog(cformat, ...)           qnndsp_base_log(NN_LOG_INFOLVL, cformat "\n", ##__VA_ARGS__)
#define i_infolog(cformat, ...)         qnndsp_base_log(NN_LOG_INFOLVL_INTERNAL, cformat "\n", ##__VA_ARGS__)
#define _debuglog(cformat, ...)         qnndsp_base_log(NN_LOG_DEBUGLVL, cformat "\n", ##__VA_ARGS__)
#define verboselog(cformat, ...)        qnndsp_base_log(NN_LOG_VERBOSELVL, cformat "\n", ##__VA_ARGS__)
#define logmsgraw(prio, cformat, ...)   (void)(qnndsp_base_log(prio, cformat, ##__VA_ARGS__))
#define logmsg(prio, cformat, ...)                                                                                     \
    (void)(qnndsp_base_log(prio, "%s:" STRINGIZE(__LINE__) ":" cformat "\n", FILE_BASENAME, ##__VA_ARGS__))
#define logmsgl(prio, cformat, ...) (void)(qnndsp_base_log(prio, cformat, ##__VA_ARGS__))

#else // ENABLE_QNNDSP_LOG

// Standalone HexNN default log

#if defined(NN_LOG_DYNLVL) && (NN_LOG_DYNLVL > 0)

// Dynamic logging level test function.
static inline bool log_condition(const int prio)
{
    return (prio <= GetLogPriorityLevel());
};

#elif defined(NN_LOG_MAXLVL)

// Logging level is fixed at compile time.
constexpr static bool log_condition(const int prio)
{
    return ((prio <= NN_LOG_MAXLVL) ? true : false);
};

#else

// Logging is completely disabled.
constexpr static bool log_condition(const int prio)
{
    return false;
};

#endif

// These are conditional, where the condition is set via compile flags.  Note that these are
// template functions so that we can exclude them from coverage using lcov commands.

template <typename... Types> inline void logmsgraw(const int prio, char const *fmt, Types... args)
{
    // LCOV_EXCL_START [SAFTYSWCCB-996]
    if (log_condition(prio)) {
        nn_log_printf(fmt, args...);
    }
    // LCOV_EXCL_STOP
}

// These macros are what are used in actual code, so that the line and filename macros will expand
// properly to show where the macro is invoked.

#define rawlog(FMT, ...)  (nn_log_printf((FMT), ##__VA_ARGS__))
#define okaylog(FMT, ...) (nn_log_printf(FILE_BASENAME ":" STRINGIZE(__LINE__) ":" FMT "\n", ##__VA_ARGS__))
#define errlog(FMT, ...)                                                                                               \
    (nn_log_printf(FILE_BASENAME ":" STRINGIZE(__LINE__) ":ERROR:" FMT "\n", ##__VA_ARGS__), GraphStatus::ErrorFatal)
#define logmsg(PRIO, FMT, ...) logmsgraw(PRIO, FILE_BASENAME ":" STRINGIZE(__LINE__) ": " FMT "\n", ##__VA_ARGS__)
#define warnlog(FMT, ...)                                                                                              \
    logmsgraw(NN_LOG_WARNLVL, FILE_BASENAME ":" STRINGIZE(__LINE__) ":WARNING: " FMT "\n", ##__VA_ARGS__)
#define statlog(statname, statvalue)                                                                                   \
    logmsgraw(NN_LOG_STATLVL, FILE_BASENAME ":" STRINGIZE(__LINE__) ":STAT: %s=%lld\n", statname, (long long)statvalue)
#define i_statlog(statname, statvalue)                                                                                 \
    logmsgraw(NN_LOG_STATLVL_INTERNAL, FILE_BASENAME ":" STRINGIZE(__LINE__) ":STAT: %s=%lld\n", statname,             \
              (long long)statvalue)
#define statslog(statname, statvalue)                                                                                  \
    logmsgraw(NN_LOG_STATLVL, FILE_BASENAME ":" STRINGIZE(__LINE__) ":STAT: %s=%s\n", statname, statvalue)
#define i_statslog(statname, statvalue)                                                                                \
    logmsgraw(NN_LOG_STATLVL_INTERNAL, FILE_BASENAME ":" STRINGIZE(__LINE__) ":STAT: %s=%s\n", statname, (statvalue))
#define infolog(FMT, ...) logmsgraw(NN_LOG_INFOLVL, FILE_BASENAME ":" STRINGIZE(__LINE__) ":" FMT "\n", ##__VA_ARGS__)
#define i_infolog(FMT, ...)                                                                                            \
    logmsgraw(NN_LOG_INFOLVL_INTERNAL, FILE_BASENAME ":" STRINGIZE(__LINE__) ":" FMT "\n", ##__VA_ARGS__)
#define _debuglog(FMT, ...)                                                                                            \
    logmsgraw(NN_LOG_DEBUGLVL, FILE_BASENAME ":" STRINGIZE(__LINE__) ":" FMT "\n", ##__VA_ARGS__)
#define verboselog(FMT, ...)                                                                                           \
    logmsgraw(NN_LOG_VERBOSELVL, FILE_BASENAME ":" STRINGIZE(__LINE__) ":" FMT "\n", ##__VA_ARGS__)

#endif // ENABLE_QNNDSP_LOG

#define debuglog(...) _debuglog(__VA_ARGS__)

#ifdef NN_LOG_MAXLVL
#define LOG_STAT()    ((NN_LOG_MAXLVL) >= NN_LOG_STATLVL)
#define LOG_INFO()    ((NN_LOG_MAXLVL) >= NN_LOG_INFOLVL)
#define LOG_DEBUG()   ((NN_LOG_MAXLVL) >= NN_LOG_DEBUGLVL)
#define LOG_VERBOSE() ((NN_LOG_MAXLVL) >= NN_LOG_VERBOSELVL)
#else
#define LOG_STAT()    (1)
#define LOG_INFO()    (1)
#define LOG_DEBUG()   (1)
#define LOG_VERBOSE() (1)
#endif //#ifdef NN_LOG_MAXLVL

class ExternalProgressLogger {

  public:
    static void start(const char *stage_name);

    static void update_progress(unsigned int numerator, unsigned int denominator);

    static void end(const char *stage_name, const char *duration);
};

class ExternalTimePoint {
    using TimePoint = std::chrono::high_resolution_clock::time_point;
    const std::string stage_name;
    const TimePoint start_time;
    unsigned int numerator = 1;
    unsigned int denominator = 1;
    bool done = false;

  public:
    explicit ExternalTimePoint(const std::string &&stage_name);

    void update_progress(unsigned int new_numerator, unsigned int new_denominator);

    void close();

    // Custom destructor
    ExternalTimePoint() = delete;
    ExternalTimePoint(const ExternalTimePoint &) = delete;
    ExternalTimePoint &operator=(ExternalTimePoint &t) = delete;
    ExternalTimePoint(ExternalTimePoint &&) = delete;
    ExternalTimePoint &operator=(ExternalTimePoint &&t) = delete;
    ~ExternalTimePoint() { close(); } // LCOV_EXCL_LINE [SAFTYSWCCB-1542]
};

#endif //#ifndef LOG_H
