#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

#include "common/Helpers.hpp"

#ifndef COMMON_ORDER_UPDATE_HPP
#define COMMON_ORDER_UPDATE_HPP

namespace common {

// Represents a normalized OrderUpdate
struct OrderUpdate {
  std::string id;     // Unique ID of the Order
  std::string symbol; // "AAPL"
  OrderAction action;

  // For ADD
  OrderSide side;

  // For ADD and MODIFY
  double price = 0.0;
  int qty = 0;

  bool isUserOrder = false;
  bool isFilled = false;

  void print() const {
    // Allow 2 decimals
    std::cout << std::fixed << std::setprecision(2);
    if (action == OrderAction::ADD) {
      std::cout << " ADD: " << id << " " << symbol << (side == OrderSide::BUY ? " BUY " : " SELL ") << "price=$"
                << price << " qty=" << qty << (isUserOrder ? " [USER]" : "");
    } else if (action == OrderAction::MODIFY) {
      std::cout << " MODIFY: " << id << " " << symbol << " price=$" << price << " qty=" << qty;
    } else if (action == OrderAction::REMOVE) {
      std::cout << " REMOVE: " << id << " " << symbol << (isFilled ? " [FILLED]" : " [CANCELLED]");
    }
    std::cout << "\n" << std::endl;
  }
};

} // namespace common

#endif
