#include <string>
#include <unordered_map>

#include "common/MarketStats.hpp"
#include "common/TradeEvent.hpp"

#ifndef MP_DATA_MANAGER_HPP
#define MP_DATA_MANAGER_HPP

namespace mp {

/* MarketDataStream
 * - aggregates Market Stats for each symbol
 */
class MarketDataManager {
public:
  // Handles an incoming TradeEvent from the MarketFeedHandler
  auto onTradeEvent(const common::TradeEvent &event) -> void;

  // Handles an update to the OrderBook for a symbol
  auto onOrderBookUpdate(const std::string &symbol, double bidPrice, double askPrice) -> void;

  // Gets the stats for the current symbol
  auto getStats(const std::string &symbol) -> common::MarketStats;

private:
  // Maps Symbol -> MarketStats (VWAP, price, volume, bid/ask)
  std::unordered_map<std::string, common::MarketStats> mStats;
};
} // namespace mp

#endif
