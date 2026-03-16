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
  using TradeCallback = std::function<void(const common::TradeEvent &)>;
  using OrderCallback = std::function<void(const common::OrderUpdate &)>;

  // Handle a TRADE message
  auto processTrade(const std::string &symbol, double price, int qty, const std::string &sellSideId,
                    const std::string &buySideId) -> void;

  // Handles an ADD order message
  auto proccessOrder(const std::string &id, const std::string &symbol, const std::string &side, double price, int qty,
                     bool isUserOrder) -> void;

  // Handles an MODIFY order message
  auto proccessOrder(const std::string &id, const std::string &symbol, double price, int qty) -> void;

  // Handles an REMOVE order message
  auto proccessOrder(const std::string &id, const std::string &symbol) -> void;

  // Subscription callback on Trade Events
  auto subscribe(TradeCallback &&callback) -> void;

  // Subscription callback on Order Updates
  auto subscribe(OrderCallback &&callback) -> void;

private:
  // Notify all subcribers of the new TradeEvent
  auto publish(const common::TradeEvent &trade) -> void;

  // Notify all subcribers of the new OrderUpdate
  auto publish(const common::OrderUpdate &order) -> void;

  // Callback that will Subscribe to new TradeEvents
  TradeCallback mTradeCallback;

  // Callback that will Subscribe to new OrderUpdates
  OrderCallback mOrderCallback;
};

} // namespace trade

#endif
