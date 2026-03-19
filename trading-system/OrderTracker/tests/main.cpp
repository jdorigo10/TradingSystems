#include "trade/OrderTracker.hpp"

int main() {
  trade::OrderTracker tracker;

  tracker.onOrderUpdate({"1", "AAPL", common::OrderAction::ADD, common::OrderSide::BUY, 100.50, 20, true});
  tracker.onOrderUpdate({"1", "AAPL", common::OrderAction::MODIFY, common::OrderSide::BUY, 100.50, 5, true});
  tracker.onOrderUpdate({"1", "AAPL", common::OrderAction::MODIFY, common::OrderSide::BUY, 100.50, 0, true});

  tracker.onOrderUpdate({"2", "MSFT", common::OrderAction::ADD, common::OrderSide::SELL, 100.40, 5, true});
  tracker.onOrderUpdate({"2", "MSFT", common::OrderAction::REMOVE, common::OrderSide::SELL, 100.40, 5, true});

  return 0;
}