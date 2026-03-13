#include <list>
#include <map>
#include <string>
#include <unordered_map>

#include "common/Order.hpp"
#include "common/OrderUpdate.hpp"

#ifndef MP_ORDER_BOOK_HPP
#define MP_ORDER_BOOK_HPP

namespace mp {

/* Order Book
 * - recieves OrderUpdate's from the MarketFeedHandler
 * - maintains the current Bids and Asks for a symbol
 */
class OrderBook {
public:
  // Handles an incoming OrderUpdate (from the MarketFeedHandler)
  auto onOrderUpdate(const common::OrderUpdate &update) -> void;

  // Prints out a snapshot of the Best Bid and Ask
  auto bookSnapshot(const std::string &symbol) const -> void;

private:
  // Handles an ADD Order Update
  auto addOrder(const common::OrderUpdate &update) -> void;

  // Handles an MODIFY Order Update
  auto modifyOrder(const common::OrderUpdate &update) -> void;

  // Handles an REMOVE Order Update
  auto removeOrder(const common::OrderUpdate &update) -> void;

  // Helper to handle removing an Order from price level
  auto removeFromPriceLevel(common::Order &update) -> void;

  // Map of OrderIDs -> Order (allows for efficient MODIFY and REMOVE)
  std::unordered_map<std::string, common::Order> mOrderLookup;

  // Map of BUY Prices -> FIFO list of Orders (pointer)
  std::map<double, std::list<common::Order *>> mBids;

  // Map of SELL Prices -> FIFO list of Orders (pointer)
  std::map<double, std::list<common::Order *>> mAsks;
};

} // namespace mp

#endif
