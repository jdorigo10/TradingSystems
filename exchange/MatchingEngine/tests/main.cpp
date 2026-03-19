#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "exchange/MatchingEngine.hpp"

int main() {
  auto orderCb = [](const std::string &id, const std::string &symbol, const common::OrderAction &action,
                    const common::OrderSide &side, double price, int qty, bool isUserOrder, bool isFilled) {
    // Allow 2 decimals
    std::cout << std::fixed << std::setprecision(2);
    if (action == common::OrderAction::ADD) {
      std::cout << "ADD " << id << " " << symbol << " ";
      std::cout << (side == common::OrderSide::BUY ? "BUY " : "SELL ");
      std::cout << price << " " << qty << (isUserOrder ? " [USER]" : "");
    } else if (action == common::OrderAction::MODIFY) {
      std::cout << "MODIFY " << id << " " << symbol << " ";
      std::cout << price << " " << qty;
    } else if (action == common::OrderAction::REMOVE) {
      std::cout << "REMOVE " << id << " " << symbol << (isFilled ? " [FILLED]" : "");
    }
    std::cout << std::endl;
  };
  auto tradeCb = [](const std::string &symbol, double price, int qty, const std::string &sellSideId,
                    const std::string &buySideId) {
    // Allow 2 decimals
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "TRADE " << symbol << " ";
    std::cout << price << " " << qty << " ";
    std::cout << sellSideId << " " << buySideId;
    std::cout << std::endl;
  };

  exchange::MatchingEngine engine("AAPL", std::move(orderCb), std::move(tradeCb));

  // Adds order 1 to book, no trade
  engine.addOrder("ORDER1", {"AAPL", common::OrderSide::BUY, 175.30, 150, true});
  std::cout << "\n";

  // Adds order 2 to book, no trade
  engine.addOrder("ORDER2", {"AAPL", common::OrderSide::SELL, 175.35, 50, false});
  std::cout << "\n";

  // Adds order 3 to book, trade with order 1, modifies order 1, removes order 3
  engine.addOrder("ORDER3", {"AAPL", common::OrderSide::SELL, 175.30, 100, false});
  std::cout << "\n";

  // Modifies order 1, no trade
  engine.modifyOrder("ORDER1", {"AAPL", common::OrderSide::BUY, 175.30, 25, true});
  std::cout << "\n";

  // Removes order 2
  engine.cancelOrder("ORDER2");
  std::cout << "\n";

  return 0;
}