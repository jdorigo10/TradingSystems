#include <optional>
#include <string>
#include <unordered_map>

#include "common/LiveOrder.hpp"
#include "common/OrderUpdate.hpp"
#include "common/TradeEvent.hpp"

#ifndef TRADE_ORDER_TRACKER_HPP
#define TRADE_ORDER_TRACKER_HPP

namespace trade {

/* OrderTracker
 * - maintains lifecycle of each of OUR orders
 */
class OrderTracker {
public:
  // Handles an OrderUpdate (will return early if not our Order)
  auto onOrderUpdate(const common::OrderUpdate &update) -> void;

  // Gets our Order from a given trade (if involved)
  auto getOrderFromTrade(const common::TradeEvent &event) const -> std::optional<common::LiveOrder>;

private:
  // Helper to handle an ADD order (new order)
  auto addOrder(const common::OrderUpdate &update) -> void;

  // Helper to handle a MODIFY order (fill order)
  auto modifyOrder(const std::string &id, int qty) -> void;

  // Helper to handle a REMOVE order (cancel order)
  auto removeOrder(const std::string &id) -> void;

  // Map of OrderId -> Live Order
  std::unordered_map<std::string, common::LiveOrder> mOrders;
};
} // namespace trade

#endif
