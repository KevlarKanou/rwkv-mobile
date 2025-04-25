#include "logger.h"
#include <string>
#include <cstdarg>

namespace rwkvmobile {

const char *level_str[] = {
    "[DEBUG]",
    "[INFO]",
    "[WARN]",
    "[ERROR]",
};

#if defined(__ANDROID__)
#include <android/log.h>
#define LOG_TAG "RWKV-MOBILE"
void Logger::log(const std::string &msg, const int level) {
    _log(std::string(level_str[level]) + " " + msg);
    if (level >= _level) {
        switch (level) {
            case RWKV_LOG_LEVEL_DEBUG:
                __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "%s", msg.c_str());
                break;
            case RWKV_LOG_LEVEL_WARN:
                __android_log_print(ANDROID_LOG_WARN, LOG_TAG, "%s", msg.c_str());
                break;
            case RWKV_LOG_LEVEL_ERROR:
                __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%s", msg.c_str());
                break;
            case RWKV_LOG_LEVEL_INFO:
            default:
                __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%s", msg.c_str());
                break;
        }
    }
}
#else
#include <cstdio>
void Logger::log(const std::string &msg, const int level) {
    _log(std::string(level_str[level]) + " " + msg);
    if (level >= _level) {
        printf("%s %s\n", level_str[level], msg.c_str());
    }
}
#endif

static Logger logger;

#define LOG_FORMAT(p, fmt, ...) \
    va_list args; \
    va_start(args, fmt); \
    vsnprintf(p, 1024, fmt, args); \
    va_end(args);

void LOGI(const char *fmt, ...) {
    char *p = new char[1024];
    LOG_FORMAT(p, fmt, args);
    logger.log(p, RWKV_LOG_LEVEL_INFO);
    delete[] p;
}

void LOGD(const char *fmt, ...) {
    char *p = new char[1024];
    LOG_FORMAT(p, fmt, args);
    logger.log(p, RWKV_LOG_LEVEL_DEBUG);
    delete[] p;
}

void LOGW(const char *fmt, ...) {
    char *p = new char[1024];
    LOG_FORMAT(p, fmt, args);
    logger.log(p, RWKV_LOG_LEVEL_WARN);
    delete[] p;
}

void LOGE(const char *fmt, ...) {
    char *p = new char[1024];
    LOG_FORMAT(p, fmt, args);
    logger.log(p, RWKV_LOG_LEVEL_ERROR);
    delete[] p;
}

void Logger::_log(const std::string &msg) {
    std::lock_guard<std::mutex> lock(_mutex);
    _buffer[_buffer_end] = msg;
    _buffer_end = (_buffer_end + 1) % LOG_BUFFER_SIZE;
    _condition.notify_one();
}

std::string logger_get_log() {
    return logger.get_log();
}

void logger_set_loglevel(int loglevel) {
    logger.set_loglevel(loglevel);
    return;
}

}