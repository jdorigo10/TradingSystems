#include "mp/StrategyEngine.hpp"

int main() {
  mp::OrderBookManager bookManager;
  bookManager.onOrderUpdate({"1", "AAPL", common::OrderUpdate::Type::ADD, common::OrderUpdate::Side::BUY, 100.50, 20});
  bookManager.onOrderUpdate({"2", "AAPL", common::OrderUpdate::Type::ADD, common::OrderUpdate::Side::BUY, 100.50, 10});
  bookManager.onOrderUpdate({"4", "AAPL", common::OrderUpdate::Type::ADD, common::OrderUpdate::Side::SELL, 100.60, 15});
  bookManager.onOrderUpdate({"5", "AAPL", common::OrderUpdate::Type::ADD, common::OrderUpdate::Side::SELL, 100.70, 30});

  mp::MarketDataManager dataManager;
  dataManager.onTradeEvent({"AAPL", 100, 10});
  dataManager.onTradeEvent({"AAPL", 120, 15});
  dataManager.onTradeEvent({"AAPL", 80, 5});

  mp::StrategyEngine strategyEngine{dataManager, bookManager};

  bookManager.onOrderUpdate({"3", "AAPL", common::OrderUpdate::Type::ADD, common::OrderUpdate::Side::BUY, 100.40, 5});
  strategyEngine.onMarketUpdate("AAPL");

  return 0;
}