#include <iomanip>
#include <iostream>
#include <string>

#include "common/Helpers.hpp"

#ifndef COMMON_ORDER_REQUEST_HPP
#define COMMON_ORDER_REQUEST_HPP

namespace common {

struct OrderRequest {
  std::string symbol; // "AAPL"

  OrderSide side;
  OrderRequestType type;

  double price = 0.0;
  int qty = 0;

  bool isUserOrder = false;

  void print() const {
    // Allow 2 decimals
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "ORDER REQUEST: ";
    std::cout << (side == OrderSide::BUY ? "BUY " : "SELL ") << symbol << " ";
    std::cout << qty << "@$" << price << (type == OrderRequestType::MARKET ? " - MARKET" : " - LIMIT");
    std::cout << "\n" << std::endl;
  }
};

} // namespace common

#endif
