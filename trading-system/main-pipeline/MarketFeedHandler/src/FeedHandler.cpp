#include "common/Helpers.hpp"

#include "mp/FeedHandler.hpp"

namespace mp {

// Handle a TRADE message
auto FeedHandler::processTrade(const std::string &symbol, double price, int qty, std::chrono::nanoseconds exchangeTs)
    -> void {
  common::TradeEvent trade;
  trade.symbol = symbol;

  trade.price = price;
  trade.qty = qty;

  trade.exchangeTimestamp = exchangeTs;
  trade.receiveTimestamp = common::currentTime();

  // Print out new TradeEvent
  trade.print();

  // Publish TradeUpdate to all subscribers
  publish(trade);
}

// Handles an ADD order message
auto FeedHandler::proccessOrder(const std::string &id, const std::string &symbol, const std::string &side, double price,
                                int qty, std::chrono::nanoseconds exchangeTs) -> void {
  common::OrderUpdate order;
  order.id = id;
  order.symbol = symbol;
  order.type = common::OrderUpdate::Type::ADD;
  order.side = (side == "BUY" ? common::OrderUpdate::Side::BUY : common::OrderUpdate::Side::SELL);

  order.price = price;
  order.qty = qty;

  order.exchangeTimestamp = exchangeTs;
  order.receiveTimestamp = common::currentTime();

  // Print out OrderUpdate
  order.print();

  // Publish OrderUpdate to all subscribers
  publish(order);
}

// Handles an MODIFY order message
auto FeedHandler::proccessOrder(const std::string &id, const std::string &symbol, double price, int qty,
                                std::chrono::nanoseconds exchangeTs) -> void {
  common::OrderUpdate order;
  order.id = id;
  order.symbol = symbol;
  order.type = common::OrderUpdate::Type::MODIFY;

  order.price = price;
  order.qty = qty;

  order.exchangeTimestamp = exchangeTs;
  order.receiveTimestamp = common::currentTime();

  // Print out OrderUpdate
  order.print();

  // Publish OrderUpdate to all subscribers
  publish(order);
}

// Handles an REMOVE order message
auto FeedHandler::proccessOrder(const std::string &id, const std::string &symbol, std::chrono::nanoseconds exchangeTs)
    -> void {
  common::OrderUpdate order;
  order.id = id;
  order.symbol = symbol;
  order.type = common::OrderUpdate::Type::REMOVE;

  order.exchangeTimestamp = exchangeTs;
  order.receiveTimestamp = common::currentTime();

  // Print out OrderUpdate
  order.print();

  // Publish OrderUpdate to all subscribers
  publish(order);
}

// Notify all subcribers of the new TradeEvent
auto FeedHandler::publish(const common::TradeEvent &trade) -> void {
  for (auto &subscriber : mTradeSubscribers) {
    subscriber(trade);
  }
}

// Notify all subcribers of the new OrderUpdate
auto FeedHandler::publish(const common::OrderUpdate &order) -> void {
  for (auto &subscriber : mOrderSubscribers) {
    subscriber(order);
  }
}

} // namespace mp
