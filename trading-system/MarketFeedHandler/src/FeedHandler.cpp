#include "common/Helpers.hpp"

#include "trade/FeedHandler.hpp"

namespace trade {

auto FeedHandler::proccessOrder(const std::string &id, const std::string &symbol, const common::OrderAction &action,
                                const common::OrderSide &side, double price, int qty, bool isUserOrder, bool isFilled)
    -> void {
  common::OrderUpdate order{id, symbol, action, side, price, qty, isUserOrder, isFilled};

  // Print out OrderUpdate
  order.print();

  // Publish the OrderUpdate
  publish(order);
}

auto FeedHandler::processTrade(const std::string &symbol, double price, int qty, const std::string &sellSideId,
                               const std::string &buySideId) -> void {
  common::TradeEvent trade{symbol, price, qty, sellSideId, buySideId};

  // Print out new TradeEvent
  trade.print();

  // Publish the TradeUpdate
  publish(trade);
}

auto FeedHandler::subscribe(FeedHandler::OrderCallback &&callback) -> void { mOrderCallback = std::move(callback); }

auto FeedHandler::subscribe(FeedHandler::TradeCallback &&callback) -> void { mTradeCallback = std::move(callback); }

auto FeedHandler::publish(const common::OrderUpdate &order) -> void {
  if (mOrderCallback) {
    mOrderCallback(order);
  }
}

auto FeedHandler::publish(const common::TradeEvent &trade) -> void {
  if (mTradeCallback) {
    mTradeCallback(trade);
  }
}

} // namespace trade
