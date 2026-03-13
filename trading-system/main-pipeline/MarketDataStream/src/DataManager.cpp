#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include <mp/DataManager.hpp>

namespace mp {

auto MarketDataManager::onTradeEvent(const common::TradeEvent &event) -> void {
  // Ignore trades that dont have any qty
  if (event.qty == 0) {
    return;
  }

  // Access/create stats for symbol
  auto &stats = mStats[event.symbol];

  stats.lastTradePrice = event.price;

  stats.priceVolume += (event.price * event.qty);
  stats.totalVolume += event.qty;
  stats.vwap = (stats.totalVolume == 0) ? 0.0 : (stats.priceVolume / stats.totalVolume);

  stats.tradeCount++;

  stats.print(event.symbol);
}

auto MarketDataManager::getStats(const std::string &symbol) const -> std::optional<common::MarketStats> {
  auto it = mStats.find(symbol);
  if (it == mStats.end()) {
    return std::nullopt;
  }

  return it->second;
}

} // namespace mp
