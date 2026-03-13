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

  // Gets the Best Bid for a given Symbol (if one)
  auto getBestBid(const std::string &symbol) const -> std::optional<common::Order>;

  // Gets the Best Ask for a given Symbol (if one)
  auto getBestAsk(const std::string &symbol) const -> std::optional<common::Order>;

private:
  // Gives a snap shot for a symbols order book
  auto snapshot(const std::string &symbol) -> void;

  std::unordered_map<std::string, std::unique_ptr<OrderBook>> mBooks;
};

} // namespace mp

#endif