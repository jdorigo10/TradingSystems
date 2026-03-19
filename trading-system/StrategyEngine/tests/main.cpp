#include "trade/StrategyEngine.hpp"

int main() {
  trade::OrderBookManager bookManager;
  bookManager.onOrderUpdate({"1", "AAPL", common::OrderAction::ADD, common::OrderSide::BUY, 100.50, 20});
  bookManager.onOrderUpdate({"2", "AAPL", common::OrderAction::ADD, common::OrderSide::BUY, 100.50, 10});
  bookManager.onOrderUpdate({"4", "AAPL", common::OrderAction::ADD, common::OrderSide::SELL, 100.60, 15});
  bookManager.onOrderUpdate({"5", "AAPL", common::OrderAction::ADD, common::OrderSide::SELL, 100.70, 30});

  trade::MarketDataManager dataManager;
  dataManager.onTradeEvent({"AAPL", 100, 10});
  dataManager.onTradeEvent({"AAPL", 120, 15});
  dataManager.onTradeEvent({"AAPL", 80, 5});

  trade::StrategyEngine strategyEngine{dataManager, bookManager};

  bookManager.onOrderUpdate({"3", "AAPL", common::OrderAction::ADD, common::OrderSide::BUY, 100.40, 5});
  strategyEngine.generateOrder("AAPL");

  return 0;
}