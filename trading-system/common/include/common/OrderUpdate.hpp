#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

namespace common {

// Represents a normalized OrderUpdate
struct OrderUpdate {
  std::string id;     // Unique ID of the Order
  std::string symbol; // "AAPL"
  enum class Type { ADD, MODIFY, REMOVE } type;

  // For ADD
  enum class Side { BUY, SELL } side;

  // For ADD and MODIFY
  double price = 0.0;
  int qty = 0;

  std::chrono::nanoseconds exchangeTimestamp; // time the exchange generated the message
  std::chrono::nanoseconds receiveTimestamp;  // time our system recieved message

  void print() const {
    // Allow 2 decimals
    std::cout << std::fixed << std::setprecision(2);
    if (type == Type::ADD) {
      std::cout << "ADD: " << id << " " << symbol << (side == Side::BUY ? " BUY " : " SELL ") << "price=" << price
                << " qty=" << qty;
    } else if (type == Type::MODIFY) {
      std::cout << "MODIFY: " << id << " " << symbol << " price=" << price << " qty=" << qty;
    } else if (type == Type::REMOVE) {
      std::cout << "REMOVE: " << id << " " << symbol;
    }
    std::cout << "\n" << std::endl;
  }
};

} // namespace common
