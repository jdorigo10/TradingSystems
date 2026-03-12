#include <iostream>
#include <sstream>
#include <string>

#include "MatchingEngine.hpp"

int main() {
  MatchingEngine engine;

  std::string line;

  // Read commands from stdin until EOF.
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    std::cout << "+----- Incoming Request -----+\n       " << line << "\n\n";
    std::stringstream ss(line);

    int id;
    std::string cmd;
    ss >> id >> cmd;

    if (cmd == "BUY") {
      int price, qty;
      ss >> price >> qty;

      engine.submitBuyOrder(id, price, qty);
    } else if (cmd == "SELL") {
      int price, qty;
      ss >> price >> qty;

      engine.submitSellOrder(id, price, qty);
    } else if (cmd == "CANCEL") {
      engine.cancel(id);
    } else {
      std::cout << "Unknown command\n";
    }
  }

  return 0;
}