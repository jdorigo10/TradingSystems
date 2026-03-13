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
};

} // namespace common

#endif
