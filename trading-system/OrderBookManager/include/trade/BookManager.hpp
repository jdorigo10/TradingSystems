#include <memory>
#include <optional>
#include <unordered_map>

#include "common/OrderUpdate.hpp"
#include "common/TradeEvent.hpp"

#include "trade/OrderBook.hpp"

#ifndef TRADE_BOOK_MANAGER_HPP
#define TRADE_BOOK_MANAGER_HPP

namespace trade {

/* Order Book Manager
 * - facilitates MarketFeedHandler Order Updates to the correct Book (via symbol)
 * - maintains the current books for all symbols
 */
class OrderBookManager {
public:
  // Facilitates an OrderUpdate to the correct Book
  auto onOrderUpdate(const common::OrderUpdate &update) -> void;

  // Gets the Book for a given symbol
  auto getBook(const std::string &symbol) -> std::optional<OrderBook>;

private:
  // Gives a snapshot for a symbols order book
  auto snapshot(const std::string &symbol) -> void;

  // Map of Symbol -> OrderBook
  std::unordered_map<std::string, OrderBook> mBooks;
};

} // namespace trade

#endif