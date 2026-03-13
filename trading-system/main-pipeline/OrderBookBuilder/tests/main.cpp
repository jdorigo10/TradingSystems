#include "mp/OrderBook.hpp"

int main() {
  mp::OrderBook book;

  book.onOrderUpdate({"1", "AAPL", common::OrderUpdate::Type::ADD, common::OrderUpdate::Side::BUY, 100.50, 20});
  book.onOrderUpdate({"2", "AAPL", common::OrderUpdate::Type::ADD, common::OrderUpdate::Side::BUY, 100.50, 10});
  book.onOrderUpdate({"3", "AAPL", common::OrderUpdate::Type::ADD, common::OrderUpdate::Side::BUY, 100.40, 5});

  book.onOrderUpdate({"4", "AAPL", common::OrderUpdate::Type::ADD, common::OrderUpdate::Side::SELL, 100.60, 15});
  book.onOrderUpdate({"5", "AAPL", common::OrderUpdate::Type::ADD, common::OrderUpdate::Side::SELL, 100.70, 30});

  book.bookSnapshot("AAPL");

  return 0;
}