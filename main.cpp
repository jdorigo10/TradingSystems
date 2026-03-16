#include <iostream>
#include <sstream>

#include "common/Helpers.hpp"

#include "trade/BookManager.hpp"
#include "trade/DataManager.hpp"
#include "trade/FeedHandler.hpp"
#include "trade/OrderTracker.hpp"
#include "trade/PositionTracker.hpp"
#include "trade/StrategyEngine.hpp"

int main() {
  trade::FeedHandler handler;
  trade::OrderBookManager bookManager;
  trade::MarketDataManager dataManager;
  trade::StrategyEngine strategyEngine{dataManager, bookManager};
  trade::OrderTracker orderTracker;
  trade::PositionTracker positionTracker;

  // Callback on each OrderUpdate from the exchange
  handler.subscribe([&](const common::OrderUpdate &update) {
    // Update the Order Book
    bookManager.onOrderUpdate(update);

    // Update our tracking for the Order (only for our Orders)
    orderTracker.onOrderUpdate(update);

    // Evaluate Symbol for an Order Request
    auto result = strategyEngine.generateOrder(update.symbol);
    if (result.has_value()) {
      auto orderRequest = std::move(result).value();

      // Evaluate risk of Order Request
      // if (riskManager.evaluateOrder(orderRequest)) {
      //   // Ensure we are not sending too many Orders to exchange
      //   if (rateLimiter.evaluateOrder(orderRequest)) {
      //     // Send allowed Order Requests to exchange (MatchingEngine)
      //     orderGateway.sendOrderRequest(orderRequest);
      //
      //     // Would usually update the OrderBook and OrderTracker here
      //     // but for simplicity will do that on the corresponding OrderUpdates
      //   }
      // }
    }
  });

  // Callback on each TradeEvent from the exchange
  handler.subscribe([&](const common::TradeEvent &event) {
    // Update Trade Stats for the Symbol
    dataManager.onTradeEvent(event);

    // Update the latest market price for this Symbol
    positionTracker.updateMarketPrice(event.symbol, event.price);

    auto order = orderTracker.getOrderFromTrade(event);
    if (order.has_value()) {
      // Update our Position for the Symbol
      positionTracker.onTradeEvent(order->id, event);
      //
      //   // Evaluate our trade and position for a Hedge Request
      //   auto result = offsetHedger.generateOrder(order->id, event);
      //   if (result.has_value()) {
      //     auto hedgeRequest = std::move(result).value();
      //
      //     // Evaluate risk of Hedge Request
      //     if (riskManager.evaluateOrder(hedgeRequest)) {
      //       // Ensure we are not sending too many Orders to exchange
      //       if (rateLimiter.evaluateOrder(hedgeRequest)) {
      //         // Send allowed Order Requests to exchange (MatchingEngine)
      //         orderGateway.sendOrderRequest(hedgeRequest);
      //
      //         // Would usually update the OrderBook and OrderTracker here
      //         // but for simplicity will do that on the corresponding OrderUpdates
      //       }
      //     }
      //   }
    }
  });

  // Read commands from stdin until EOF.
  std::string line;
  while (getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    std::cout << "\n-----> Incoming Data: " << line << "\n\n";
    std::stringstream ss(line);

    std::string type;
    ss >> type;

    if (type == "TRADE") {
      std::string symbol;
      double price;
      int qty;
      std::string sellId;
      std::string buyId;
      ss >> symbol >> price >> qty >> sellId >> buyId;

      handler.processTrade(symbol, price, qty, sellId, buyId);
    } else if (type == "ADD") {
      std::string id;
      std::string symbol;
      std::string side;
      double price;
      int qty;
      bool isUserOrder;
      ss >> id >> symbol >> side >> price >> qty >> isUserOrder;

      handler.proccessOrder(id, symbol, side, price, qty, isUserOrder);
    } else if (type == "MODIFY") {
      std::string id;
      std::string symbol;
      double price;
      int qty;
      ss >> id >> symbol >> price >> qty;

      handler.proccessOrder(id, symbol, price, qty);
    } else if (type == "REMOVE") {
      std::string id;
      std::string symbol;
      ss >> id >> symbol;

      handler.proccessOrder(id, symbol);
    } else {
      std::cout << "UNKNOWN TYPE" << std::endl;
    }
  }

  return 0;
}
