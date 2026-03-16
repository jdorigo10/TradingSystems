#include <chrono>

#ifndef COMMON_HELPERS_HPP
#define COMMON_HELPERS_HPP

namespace common {

// Helper for getting current time (ns)
inline auto currentTime() -> std::chrono::nanoseconds {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch());
}

// ENUM for an OrderSide
enum class OrderSide { BUY, SELL };

// ENUM for an OrderUpdateType
enum class OrderUpdateType { ADD, MODIFY, REMOVE };

// ENUM for an OrderRequestType
enum class OrderRequestType { MARKET, LIMIT };

// ENUM for a LiveOrderStatus
enum class LiveOrderStatus { NEW, PARTIALLY_FILLED, FILLED, CANCELLED };

} // namespace common

#endif
