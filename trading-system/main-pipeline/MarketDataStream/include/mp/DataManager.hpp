#include <optional>
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

  // Gets the stats for the current symbol
  auto getStats(const std::string &symbol) const -> std::optional<common::MarketStats>;

private:
  // Maps Symbol -> MarketStats (VWAP, price, volume, bid/ask)
  std::unordered_map<std::string, common::MarketStats> mStats;
};
} // namespace mp

#endif
