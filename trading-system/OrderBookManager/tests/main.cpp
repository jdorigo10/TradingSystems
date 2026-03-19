#include "trade/OrderBook.hpp"

int main() {
  trade::OrderBook book;

  book.onOrderUpdate({"1", "AAPL", common::OrderAction::ADD, common::OrderSide::BUY, 100.50, 20});
  book.snapshot();
  book.onOrderUpdate({"2", "AAPL", common::OrderAction::ADD, common::OrderSide::BUY, 100.50, 10});
  book.snapshot();
  book.onOrderUpdate({"3", "AAPL", common::OrderAction::ADD, common::OrderSide::BUY, 100.40, 5});
  book.snapshot();

  book.onOrderUpdate({"4", "AAPL", common::OrderAction::ADD, common::OrderSide::SELL, 100.60, 15});
  book.snapshot();
  book.onOrderUpdate({"5", "AAPL", common::OrderAction::ADD, common::OrderSide::SELL, 100.70, 30});
  book.snapshot();

  return 0;
}