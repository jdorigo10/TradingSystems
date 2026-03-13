#include <chrono>
#include <functional>
#include <string>
#include <vector>

#include "common/OrderUpdate.hpp"
#include "common/TradeEvent.hpp"

#ifndef MP_FEED_HANDLER_HPP
#define MP_FEED_HANDLER_HPP

namespace mp {

/* Market Data Feed Handler
 * - recieves raw market messages (TradeEvents, OrderUpdates)
 * - parses/normalizes them into a standard internal format
 * - forwards them to downstream systems (OrderBookBuilder, MarketDataStream, EventBus)
 */
class FeedHandler {
public:
  // Callback function for subscribers
  using TradeCallback = std::function<void(const common::TradeEvent &)>;
  using OrderCallback = std::function<void(const common::OrderUpdate &)>;

  // Handle a TRADE message
  auto processTrade(const std::string &symbol, double price, int qty, std::chrono::nanoseconds exchangeTs) -> void;

  // Handles an ADD order message
  auto proccessOrder(const std::string &id, const std::string &symbol, const std::string &side, double price, int qty,
                     std::chrono::nanoseconds exchangeTs) -> void;

  // Handles an MODIFY order message
  auto proccessOrder(const std::string &id, const std::string &symbol, double price, int qty,
                     std::chrono::nanoseconds exchangeTs) -> void;

  // Handles an REMOVE order message
  auto proccessOrder(const std::string &id, const std::string &symbol, std::chrono::nanoseconds exchangeTs) -> void;

  // Subscribes to Trade Events
  auto subscribe(TradeCallback callback) -> void;

  // Subscribes to Order Updates
  auto subscribe(OrderCallback callback) -> void;

private:
  // Notify all subcribers of the new TradeEvent
  auto publish(const common::TradeEvent &trade) -> void;

  // Notify all subcribers of the new OrderUpdate
  auto publish(const common::OrderUpdate &order) -> void;

  // Downstream components that will Subscribe to new TradeEvents
  std::vector<TradeCallback> mTradeSubscribers;

  // Downstream components that will Subscribe to new OrderUpdates
  std::vector<OrderCallback> mOrderSubscribers;
};

} // namespace mp

#endif
