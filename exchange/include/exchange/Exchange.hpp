#include <optional>
#include <string>
#include <unordered_map>

#include "common/OrderRequest.hpp"

#include "exchange/MatchingEngine.hpp"

#ifndef EXCHANGE_EXCHANGE_HPP
#define EXCHANGE_EXCHANGE_HPP

namespace exchange {

class Exchange {
public:
  // Sets the Callback to be used on all Order updates
  auto setOrderCallback(MatchingEngine::OrderCb cb) -> void;

  // Sets the Callback to be used on all Trade executions
  auto setTradeCallback(MatchingEngine::TradeCb cb) -> void;

  // Handles an incoming order request (pass in an Order ID to modify/remove existing ones)
  auto processRequest(const common::OrderRequest &request, const std::optional<std::string> &id = std::nullopt) -> void;

private:
  // Helper to get or create a MatchingEngine for a given symbol
  auto getOrCreateEngine(const std::string &symbol) -> MatchingEngine &;

  // Callback for all exchange orders (ADD, MODIFY, REMOVE)
  MatchingEngine::OrderCb mOrderCallback;

  // Callback for an executed Trade
  MatchingEngine::TradeCb mTradeCallback;

  // Counter for Unique Order IDs
  int mOrderId = 1;

  // Map of Symbols to their Matching Engine
  std::unordered_map<std::string, MatchingEngine> mEngines;
};

} // namespace exchange

#endif
