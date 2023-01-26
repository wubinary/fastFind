#pragma once

#include "common/color.hpp"

#include <iostream>

__BEGIN_DECLS

enum Severity : uint8_t {
    INFO    = 0,
    WARN    = 1,
    ERROR   = 2,
    VERBOSE = 3,
};

namespace common {

namespace log {

static bool ShouldLog(Severity severity) {
    switch (severity) {
        case INFO:
        case WARN:
        case ERROR:
            return true;
        case VERBOSE:
#ifdef DEBUG
            return true;
#else
            return false;
#endif
        default:
            return false;
    }
}

}  // namespace log

}  // namespace common


#ifndef LOG
#define LOG(severity) \
    common::log::ShouldLog(severity) && LOG_STREAM(severity)
#endif

#ifndef LOG_STREAM
#ifdef DEBUG
#define LOG_STREAM(severity) \
    std::cout << __FILE__ << " (line:" << __LINE__ << ") " \
              << "[" << #severity << "] :"
#else
#define LOG_STREAM(severity) \
    std::cout << "[" << #severity << "] :"
#endif
#endif


#ifndef SHOW
#define SHOW() \
    std::cout
#endif

__END_DECLS
