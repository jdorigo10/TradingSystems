#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

#include "FeedHandler.hpp"

namespace {
auto currentTime() -> std::chrono::nanoseconds {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch());
}
} // namespace

int main() {
  FeedHandler handler;

  // Read commands from stdin until EOF.
  std::string line;
  while (getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    std::stringstream ss(line);

    std::string type;
    ss >> type;

    if (type == "TRADE") {
      std::string symbol;
      double price;
      int qty;
      ss >> symbol >> price >> qty;

      handler.processTrade(symbol, price, qty, currentTime());
    } else if (type == "ADD") {
      std::string id;
      std::string symbol;
      std::string side;
      double price;
      int qty;
      ss >> id >> symbol >> side >> price >> qty;

      handler.proccessOrder(id, symbol, side, price, qty, currentTime());
    } else if (type == "MODIFY") {
      std::string id;
      std::string symbol;
      double price;
      int qty;
      ss >> id >> symbol >> price >> qty;

      handler.proccessOrder(id, symbol, price, qty, currentTime());
    } else if (type == "REMOVE") {
      std::string id;
      std::string symbol;
      ss >> id >> symbol;

      handler.proccessOrder(id, symbol, currentTime());
    } else {
      std::cout << "UNKNOWN TYPE" << std::endl;
    }
  }

  std::cout << "\n";
  return 0;
}