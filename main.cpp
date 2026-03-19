#include <iostream>
#include <sstream>

#include "common/Helpers.hpp"

#include "exchange/Exchange.hpp"

#include "trade/BookManager.hpp"
#include "trade/DataManager.hpp"
#include "trade/FeedHandler.hpp"
#include "trade/OrderTracker.hpp"
#include "trade/PositionTracker.hpp"
#include "trade/StrategyEngine.hpp"

int main() {
  exchange::Exchange exchange;
  trade::FeedHandler handler;

  // Callback on each Exchange order ouput
  exchange.setOrderCallback([&](const std::string &id, const std::string &symbol, const common::OrderAction &action,
                                const common::OrderSide &side, double price, int qty, bool isUserOrder) {
    std::cout << "\n------------------------------------------------------------------------------------------------\n";

    // Allow 2 decimals
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "EXCHANGE --> ";
    if (action == common::OrderAction::ADD) {
      std::cout << "ADD " << id << " " << symbol << " ";
      std::cout << (side == common::OrderSide::BUY ? "BUY " : "SELL ");
      std::cout << price << " " << qty << (isUserOrder ? " [USER]" : "");
    } else if (action == common::OrderAction::MODIFY) {
      std::cout << "MODIFY " << id << " " << symbol << " ";
      std::cout << price << " " << qty;
    } else if (action == common::OrderAction::REMOVE) {
      std::cout << "REMOVE " << id << " " << symbol << " " << qty;
    }
    std::cout << std::endl;

    // Normalize incoming order, and publish to rest of trading system
    handler.proccessOrder(id, symbol, action, side, price, qty, isUserOrder);
  });

  // Callback on each Exchange trade ouput
  exchange.setTradeCallback([&](const std::string &symbol, double price, int qty, const std::string &sellSideId,
                                const std::string &buySideId) {
    std::cout << "\n------------------------------------------------------------------------------------------------\n";

    // Allow 2 decimals
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "EXCHANGE --> ";
    std::cout << "TRADE " << symbol << " ";
    std::cout << price << " " << qty << " ";
    std::cout << sellSideId << " " << buySideId;
    std::cout << std::endl;

    // Normalize incoming order, and publish to rest of trading system
    handler.processTrade(symbol, price, qty, sellSideId, buySideId);
  });

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

  /* TODO: Eventually read order requests from stdin
    // Read commands from stdin until EOF.
    std::string line;
    while (getline(std::cin, line)) {
      if (line.empty()) {
        continue;
      }
      std::cout << "\n-----> Incoming Data: " << line << "\n\n";
      std::stringstream ss(line);
    }
  */

  // Create OrderRequest for incoming data into exchange
  exchange.processRequest({"AAPL", common::OrderSide::BUY, 175.30, 150, true});
  exchange.processRequest({"AAPL", common::OrderSide::SELL, 175.30, 50, false});
  exchange.processRequest({"AAPL", common::OrderSide::SELL, 175.10, 200, false});

  return 0;
}
