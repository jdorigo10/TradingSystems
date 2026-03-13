#include <iomanip>
#include <iostream>
#include <string>

#ifndef COMMON_MARKETSTATS_HPP
#define COMMON_MARKETSTATS_HPP

namespace common {

struct MarketStats {
  // Trade metrics
  double lastTradePrice = 0.0; // Price of last trade executed
  double priceVolume = 0.0;    // Sum of (price * qty) used for VWAP
  long totalVolume = 0;        // Sum of all traded qtys
  int tradeCount = 0;          // Total number of trades
  double vwap = 0.0;           // VWAP = total price-volume / total volume

  void print(const std::string &symbol) const {
    // Allow 2 decimals
    std::cout << std::fixed << std::setprecision(2);
    std::cout << symbol << " Trade Stats\n";
    std::cout << "  lastPrice=$" << lastTradePrice << " tradeCount=" << tradeCount << " totalVolume=" << totalVolume
              << " VWAP=$" << vwap;
    std::cout << "\n" << std::endl;
  }
};

} // namespace common

#endif
