#include "trade/PositionTracker.hpp"

int main() {
  trade::PositionTracker tracker;

  common::TradeEvent trade1{"AAPL", 150, 10, "1", "2"};
  tracker.updateMarketPrice(trade1.symbol, trade1.price);
  tracker.onTradeEvent("1", trade1);

  common::TradeEvent trade2{"AAPL", 100, 20, "3", "4"};
  tracker.updateMarketPrice(trade2.symbol, trade2.price);
  tracker.onTradeEvent("3", trade2);

  common::TradeEvent trade3{"AAPL", 50, 50, "5", "6"};
  tracker.updateMarketPrice(trade3.symbol, trade3.price);
  tracker.onTradeEvent("6", trade3);

  return 0;
}