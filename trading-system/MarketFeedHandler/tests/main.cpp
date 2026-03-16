#include <iostream>
#include <sstream>

#include "common/Helpers.hpp"

#include "trade/FeedHandler.hpp"

int main() {
  trade::FeedHandler handler;

  // Read commands from stdin until EOF.
  std::string line;
  while (getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    std::cout << "+------------- Incoming Data -------------+\n    " << line << "\n\n";
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
