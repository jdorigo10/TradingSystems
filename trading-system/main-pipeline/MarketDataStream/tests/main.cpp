#include "mp/DataManager.hpp"

int main() {
  mp::MarketDataManager dataManager;

  dataManager.onTradeEvent({"AAPL", 100, 10});
  dataManager.onTradeEvent({"AAPL", 120, 15});
  dataManager.onTradeEvent({"AAPL", 80, 5});

  dataManager.onOrderBookUpdate("AAPL", 10, 12);
  dataManager.onOrderBookUpdate("MSFT", 12, 10);

  return 0;
}