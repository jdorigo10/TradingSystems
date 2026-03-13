#include <iomanip>
#include <iostream>
#include <list>
#include <string>

#ifndef COMMON_ORDER_HPP
#define COMMON_ORDER_HPP

namespace common {

struct Order {
  std::string id;
  enum class Side { BUY, SELL } side;
  double price;
  int qty;

  // iterator pointing to the order in price level list
  std::list<Order *>::iterator listIt;

  void print() const {
    // Allow 2 decimals
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "$" << price << " - qty=" << qty << " id=" << id;
  }
};

} // namespace common

#endif
