#include <iomanip>
#include <iostream>
#include <string>

#ifndef COMMON_POSITION_HPP
#define COMMON_POSITION_HPP

namespace common {

struct Position {
  int position = 0;         // Net shares (POS = long, NEG = short)
  double avgPrice = 0.0;    // Average entry price
  double realizedPnL = 0.0; // Realized Profit & Loss from closed trades
  double lastMark = 0.0;    // Latest Market Price

  double unrealizedPnL() const { return (lastMark - avgPrice) * position; }

  double totalPnL() const { return realizedPnL + unrealizedPnL(); }

  void print(const std::string &symbol) const {
    // Allow 2 decimals
    std::cout << std::fixed << std::setprecision(2);
    std::cout << " " << symbol << " Position\n";
    std::cout << "   qty=" << position << " avgPrice=$" << avgPrice << "\n";
    std::cout << "   rPnL=" << realizedPnL << " uPnL=" << unrealizedPnL() << " tPnL=" << totalPnL();
    std::cout << "\n" << std::endl;
  }
};

} // namespace common

#endif
