#include <memory>
#include <unordered_map>

#include "common/OrderUpdate.hpp"

#include "mp/OrderBook.hpp"

#ifndef MP_BOOK_MANAGER_HPP
#define MP_BOOK_MANAGER_HPP

namespace mp {

/* Order Book Manager
 * - facilitates MarketFeedHandler Order Updates to the correct Book (via symbol)
 * - maintains the current books for all symbols
 */
class OrderBookManager {
public:
  // Facilitates an OrderUpdate to the correct Book
  auto onOrderUpdate(const common::OrderUpdate &update) -> void;

private:
  // Gives a snap shot of all the order books
  auto snapshot() -> void;

  std::unordered_map<std::string, std::unique_ptr<OrderBook>> mBooks;
};

} // namespace mp

#endif