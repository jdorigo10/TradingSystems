#include <chrono>
#include <functional>
#include <string>
#include <vector>

#include "common/OrderUpdate.hpp"
#include "common/TradeEvent.hpp"

#ifndef TRADE_FEED_HANDLER_HPP
#define TRADE_FEED_HANDLER_HPP

namespace trade {

/* Market Data Feed Handler
 * - recieves raw market messages (TradeEvents, OrderUpdates)
 * - parses/normalizes them into a standard internal format
 * - forwards them to downstream systems (OrderBookManager, MarketDataManager, EventBus)
 */
class FeedHandler {
public:
  // Callback function for subscribers
  using OrderCallback = std::function<void(const common::OrderUpdate &)>;
  using TradeCallback = std::function<void(const common::TradeEvent &)>;

  // Handles an Order message
  auto proccessOrder(const std::string &id, const std::string &symbol, const common::OrderAction &action,
                     const common::OrderSide &side, double price, int qty, bool isUserOrder) -> void;

  // Handle a Trade message
  auto processTrade(const std::string &symbol, double price, int qty, const std::string &sellSideId,
                    const std::string &buySideId) -> void;

  // Subscription callback on Order Updates
  auto subscribe(OrderCallback &&callback) -> void;

  // Subscription callback on Trade Events
  auto subscribe(TradeCallback &&callback) -> void;

private:
  // Publish the new OrderUpdate
  auto publish(const common::OrderUpdate &order) -> void;

  // Publish the new TradeEvent
  auto publish(const common::TradeEvent &trade) -> void;

  // Callback that will Subscribe to new OrderUpdates
  OrderCallback mOrderCallback;

  // Callback that will Subscribe to new TradeEvents
  TradeCallback mTradeCallback;
};

} // namespace trade

#endif
