#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

#ifndef COMMON_TRADE_EVENT_HPP
#define COMMON_TRADE_EVENT_HPP

namespace common {

// Represents a normalized TradeEvent
struct TradeEvent {
  std::string symbol; // "AAPL"

  double price = 0.0;
  int qty = 0;

  std::string sellSideId; // Unique ID of the SELL Order
  std::string buySideId;  // Unique ID of the BUY Order

  void print() const {
    // Allow 2 decimals
    std::cout << std::fixed << std::setprecision(2);
    std::cout << " TRADE: " << symbol << " price=$" << price << " qty=" << qty;
    std::cout << " [" << sellSideId << " <-> " << buySideId << "]";
    std::cout << "\n" << std::endl;
  }
};

} // namespace common

#endif
