#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

namespace common {

// Represents a normalized TradeEvent
struct TradeEvent {
  std::string symbol; // "AAPL"

  double price = 0.0;
  int qty = 0;

  std::chrono::nanoseconds exchangeTimestamp; // time the exchange generated the message
  std::chrono::nanoseconds receiveTimestamp;  // time our system recieved message

  void print() const {
    // Allow 2 decimals
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "TRADE: " << symbol << " price=" << price << " qty=" << qty;
    std::cout << std::endl;
  }
};

} // namespace common
