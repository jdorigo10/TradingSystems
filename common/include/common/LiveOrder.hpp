#include <iomanip>
#include <iostream>
#include <string>

#include "common/Helpers.hpp"

#ifndef COMMON_LIVE_ORDER_HPP
#define COMMON_LIVE_ORDER_HPP

namespace common {

// Represents a Live Order being Tracked
struct LiveOrder {
  std::string id;     // Unique Order ID
  std::string symbol; // symbol: "AAPL"
  OrderSide side;

  int qty;                // Original Order quantity
  int filled = 0;         // Quantity filled so far
  double price;           // Limit price
  LiveOrderStatus status; // Current state of the order

  void print() const {
    // Allow 2 decimals
    std::cout << std::fixed << std::setprecision(2);
    std::cout << " ORDER FILL: [" << id << "] ";
    std::cout << (side == OrderSide::BUY ? "BUY " : "SELL ") << symbol << " ";
    std::cout << filled << "/" << qty << "@$" << price;
    switch (status) {
    case LiveOrderStatus::NEW:
      std::cout << " - NEW";
      break;
    case LiveOrderStatus::PARTIALLY_FILLED:
      std::cout << " - PARTIALLY_FILLED";
      break;
    case LiveOrderStatus::FILLED:
      std::cout << " - FILLED";
      break;
    case LiveOrderStatus::CANCELLED:
      std::cout << " - CANCELLED";
      break;
    }
    std::cout << "\n" << std::endl;
  }
};

} // namespace common

#endif
