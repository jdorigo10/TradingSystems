#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Helper for getting current time (ns)
auto currentTime() -> std::chrono::nanoseconds {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch());
}

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
    std::cout << std::endl;
  }
};

/* Market Data Feed Handler
 * - recieves raw market messages (TradeEvents, OrderUpdates)
 * - parses/normalizes them into a standard internal format
 * - forwards them to downstream systems (OrderBookBuilder, MarketDataStream, EventBus)
 */
class FeedHandler {
public:
  // Callback function for subscribers
  using TradeCallback = std::function<void(const TradeEvent &)>;
  using OrderCallback = std::function<void(const OrderUpdate &)>;

  // Handle a TRADE message
  auto processTrade(const std::string &symbol, double price, int qty, std::chrono::nanoseconds exchangeTs) -> void {
    TradeEvent trade;
    trade.symbol = symbol;

    trade.price = price;
    trade.qty = qty;

    trade.exchangeTimestamp = exchangeTs;
    trade.receiveTimestamp = currentTime();

    // Print out new TradeEvent
    trade.print();

    // Publish TradeUpdate to all subscribers
    publish(trade);
  }

  // Handles an ADD order message
  auto proccessOrder(const std::string &id, const std::string &symbol, const std::string &side, double price, int qty,
                     std::chrono::nanoseconds exchangeTs) -> void {
    OrderUpdate order;
    order.id = id;
    order.symbol = symbol;
    order.type = OrderUpdate::Type::ADD;
    order.side = (side == "BUY" ? OrderUpdate::Side::BUY : OrderUpdate::Side::SELL);

    order.price = price;
    order.qty = qty;

    order.exchangeTimestamp = exchangeTs;
    order.receiveTimestamp = currentTime();

    // Print out OrderUpdate
    order.print();

    // Publish OrderUpdate to all subscribers
    publish(order);
  }

  // Handles an MODIFY order message
  auto proccessOrder(const std::string &id, const std::string &symbol, double price, int qty,
                     std::chrono::nanoseconds exchangeTs) -> void {
    OrderUpdate order;
    order.id = id;
    order.symbol = symbol;
    order.type = OrderUpdate::Type::MODIFY;

    order.price = price;
    order.qty = qty;

    order.exchangeTimestamp = exchangeTs;
    order.receiveTimestamp = currentTime();

    // Print out OrderUpdate
    order.print();

    // Publish OrderUpdate to all subscribers
    publish(order);
  }

  // Handles an REMOVE order message
  auto proccessOrder(const std::string &id, const std::string &symbol, std::chrono::nanoseconds exchangeTs) -> void {
    OrderUpdate order;
    order.id = id;
    order.symbol = symbol;
    order.type = OrderUpdate::Type::REMOVE;

    order.exchangeTimestamp = exchangeTs;
    order.receiveTimestamp = currentTime();

    // Print out OrderUpdate
    order.print();

    // Publish OrderUpdate to all subscribers
    publish(order);
  }

private:
  // Notify all subcribers of the new TradeEvent
  auto publish(const TradeEvent &trade) -> void {
    for (auto &subscriber : mTradeSubscribers) {
      subscriber(trade);
    }
  }

  // Notify all subcribers of the new OrderUpdate
  auto publish(const OrderUpdate &order) -> void {
    for (auto &subscriber : mOrderSubscribers) {
      subscriber(order);
    }
  }

  // Downstream components that will Subscribe to new TradeEvents
  std::vector<TradeCallback> mTradeSubscribers;

  // Downstream components that will Subscribe to new OrderUpdates
  std::vector<OrderCallback> mOrderSubscribers;
};

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