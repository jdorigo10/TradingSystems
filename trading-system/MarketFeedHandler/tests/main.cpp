#include <iostream>
#include <sstream>

#include "common/Helpers.hpp"

#include "trade/FeedHandler.hpp"

int main() {
  trade::FeedHandler handler;

  // ADD order1 AAPL
  handler.proccessOrder("ORDER1", "AAPL", common::OrderAction::ADD, common::OrderSide::BUY, 175.30, 150, true, false);

  // ADD order2 AAPL
  handler.proccessOrder("ORDER2", "AAPL", common::OrderAction::ADD, common::OrderSide::SELL, 175.30, 50, false, false);

  // TRADE order1 order2
  handler.processTrade("AAPL", 175.30, 50, "ORDER2", "ORDER1");

  return 0;
}
