#include <iostream>
#include <sstream>

#include "common/Helpers.hpp"

#include "mp/BookManager.hpp"
#include "mp/DataManager.hpp"
#include "mp/FeedHandler.hpp"

int main() {
  mp::FeedHandler handler;
  mp::OrderBookManager bookManager;
  mp::MarketDataManager dataManager;

  // Order Books subscribe to OrderUpdates from MarketFeedHandler
  handler.subscribe([&](const common::OrderUpdate &update) {
    bookManager.onOrderUpdate(update);

    auto bestBid = bookManager.getBestBid(update.symbol);
    auto bestAsk = bookManager.getBestAsk(update.symbol);
    if (bestBid.has_value() && bestAsk.has_value()) {
      dataManager.onOrderBookUpdate(update.symbol, bestBid->price, bestAsk->price);
    }
  });

  // Market Data subscribes to TradeEvents from MarketFeedHandler
  handler.subscribe([&](const common::TradeEvent &event) { dataManager.onTradeEvent(event); });

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
      ss >> symbol >> price >> qty;

      handler.processTrade(symbol, price, qty, common::currentTime());
    } else if (type == "ADD") {
      std::string id;
      std::string symbol;
      std::string side;
      double price;
      int qty;
      ss >> id >> symbol >> side >> price >> qty;

      handler.proccessOrder(id, symbol, side, price, qty, common::currentTime());
    } else if (type == "MODIFY") {
      std::string id;
      std::string symbol;
      double price;
      int qty;
      ss >> id >> symbol >> price >> qty;

      handler.proccessOrder(id, symbol, price, qty, common::currentTime());
    } else if (type == "REMOVE") {
      std::string id;
      std::string symbol;
      ss >> id >> symbol;

      handler.proccessOrder(id, symbol, common::currentTime());
    } else {
      std::cout << "UNKNOWN TYPE" << std::endl;
    }
  }

  return 0;
}
