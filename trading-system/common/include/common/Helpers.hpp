#include <chrono>

#ifndef COMMON_HELPERS_HPP
#define COMMON_HELPERS_HPP

namespace common {

// Helper for getting current time (ns)
inline auto currentTime() -> std::chrono::nanoseconds {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch());
}

} // namespace common

#endif
