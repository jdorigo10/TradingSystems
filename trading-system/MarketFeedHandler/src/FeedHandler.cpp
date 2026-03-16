#include "common/Helpers.hpp"

#include "trade/FeedHandler.hpp"

namespace trade {

auto FeedHandler::processTrade(const std::string &symbol, double price, int qty, const std::string &sellSideId,
                               const std::string &buySideId) -> void {
  common::TradeEvent trade;
  trade.symbol = symbol;

  trade.price = price;
  trade.qty = qty;

  trade.sellSideId = sellSideId;
  trade.buySideId = buySideId;

  // Print out new TradeEvent
  trade.print();

  // Publish the TradeUpdate
  publish(trade);
}

auto FeedHandler::proccessOrder(const std::string &id, const std::string &symbol, const std::string &side, double price,
                                int qty, bool isUserOrder) -> void {
  common::OrderUpdate order;
  order.id = id;
  order.symbol = symbol;
  order.type = common::OrderUpdateType::ADD;
  order.side = (side == "BUY" ? common::OrderSide::BUY : common::OrderSide::SELL);

  order.price = price;
  order.qty = qty;

  order.isUserOrder = isUserOrder;

  // Print out OrderUpdate
  order.print();

  // Publish the OrderUpdate
  publish(order);
}

auto FeedHandler::proccessOrder(const std::string &id, const std::string &symbol, double price, int qty) -> void {
  common::OrderUpdate order;
  order.id = id;
  order.symbol = symbol;
  order.type = common::OrderUpdateType::MODIFY;

  order.price = price;
  order.qty = qty;

  // Print out OrderUpdate
  order.print();

  // Publish the OrderUpdate
  publish(order);
}

auto FeedHandler::proccessOrder(const std::string &id, const std::string &symbol) -> void {
  common::OrderUpdate order;
  order.id = id;
  order.symbol = symbol;
  order.type = common::OrderUpdateType::REMOVE;

  // Print out OrderUpdate
  order.print();

  // Publish the OrderUpdate
  publish(order);
}

auto FeedHandler::subscribe(FeedHandler::TradeCallback &&callback) -> void { mTradeCallback = std::move(callback); }

auto FeedHandler::subscribe(FeedHandler::OrderCallback &&callback) -> void { mOrderCallback = std::move(callback); }

auto FeedHandler::publish(const common::TradeEvent &trade) -> void {
  if (mTradeCallback) {
    mTradeCallback(trade);
  }
}

auto FeedHandler::publish(const common::OrderUpdate &order) -> void {
  if (mOrderCallback) {
    mOrderCallback(order);
  }
}

} // namespace trade
