#pragma once

#include <common/log.h>

#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include <vector>


// ============================================================================================== //
// LIKELY & UNLIKELY
// ============================================================================================== //

__BEGIN_DECLS

#define LIKELY(expr)   __builtin_expect(!!(expr),1)
#define UNLIKELY(expr) __builtin_expect(!!(expr),0)

__END_DECLS


// ============================================================================================== //
// OS STREAMING
// ============================================================================================== //

#define SHOW_ELE_WITH_PREFIX(os, vec, prefix) do { \
    for (const auto& ele : vec) { \
        os << prefix << ele << "\n"; \
    } \
} while (0);

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    SHOW_ELE_WITH_PREFIX(os, vec, "- ");
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<T>>& vec2d) {
    for (const auto& vec : vec2d) {
        os << "- \n";
        SHOW_ELE_WITH_PREFIX(os, vec, "-- ");
    }
    return os;
}


// =================================================================================================
// Scoped timer declaration
// =================================================================================================

__BEGIN_DECLS

#define SCOPED_TIMER() SCOPED_TIMER_CB(ScopedTimer::defaultCallback);
#define SCOPED_TIMER_CB(callback) ScopedTimer scopedTimer(callback);
#define SCOPED_TIMER_SUMMARY() \
    SHOW() << __FUNCTION__ << " elapse : " << ScopedTimer::scopedTotalTime << " ms." << std::endl;

struct ScopedTimer {
    using time_p = std::chrono::high_resolution_clock::time_point;
    using milliseconds = std::chrono::milliseconds;
    using timeunit = milliseconds;

    static inline double scopedTotalTime = 0;

    static inline std::function<void(double)> defaultCallback = [](double span) {
        scopedTotalTime += span;
    };

    ScopedTimer(const std::function<void(double)> _callback) {
        callback = _callback;
        tStart = std::chrono::high_resolution_clock::now();
    }

    ~ScopedTimer() {
        auto tEnd = std::chrono::high_resolution_clock::now();
        auto span = std::chrono::duration_cast<timeunit>(tEnd - tStart).count();

        callback(span);
    }

    time_p tStart;
    std::function<void(double)> callback;
};

__END_DECLS

