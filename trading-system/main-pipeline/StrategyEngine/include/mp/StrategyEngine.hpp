#include <string>
#include <unordered_map>

#include "common/Order.hpp"
#include "common/OrderRequest.hpp"

#include "mp/BookManager.hpp"
#include "mp/DataManager.hpp"

#ifndef MP_STRATEGY_ENGINE_HPP
#define MP_STRATEGY_ENGINE_HPP

namespace mp {

/* StrategyEngine
 * - generates OrderRequests based on MarketStats and OrderBooks for a given Symbol
 */
class StrategyEngine {
public:
  explicit StrategyEngine(MarketDataManager &dataManager, OrderBookManager &bookManager)
      : mDataManager(dataManager), mBookManager(bookManager) {}

  // Checks if there is an OrderRequest to be made for a given Symbol based on Stats/Book
  auto onMarketUpdate(const std::string &symbol) -> std::optional<common::OrderRequest>;

private:
  // Helper to determine order size based on best Bid & Ask
  auto determineOrderSize(const common::Order &bestBid, const common::Order &bestAsk) -> int;

  // Map of Symbol -> MarketStats
  MarketDataManager &mDataManager;

  // Map of Symbol -> OrderBook
  OrderBookManager &mBookManager;
};
} // namespace mp

#endif
