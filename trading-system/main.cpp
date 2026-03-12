#include <iostream>
#include <sstream>

#include "common/Helpers.hpp"

#include "mp/FeedHandler.hpp"

int main() {
  mp::FeedHandler handler;

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
