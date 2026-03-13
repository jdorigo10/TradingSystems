#include <iomanip>
#include <iostream>
#include <string>

#ifndef COMMON_ORDERREQUEST_HPP
#define COMMON_ORDERREQUEST_HPP

namespace common {

struct OrderRequest {
  std::string symbol; // "AAPL"

  enum class Side { BUY, SELL } side;
  enum class Type { MARKET, LIMIT } type;

  double price = 0.0;
  int qty = 0;

  void print() const {
    // Allow 2 decimals
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "ORDER REQUEST: ";
    std::cout << (side == OrderRequest::Side::BUY ? "BUY " : "SELL ") << symbol << " ";
    std::cout << qty << "@$" << price << (type == OrderRequest::Type::MARKET ? " - MARKET" : " - LIMIT");
    std::cout << "\n" << std::endl;
  }
};

} // namespace common

#endif
