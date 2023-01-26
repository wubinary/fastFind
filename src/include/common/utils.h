#pragma once

#include <iostream>
#include <string>
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

