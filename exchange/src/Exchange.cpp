#include "exchange/Exchange.hpp"

namespace exchange {

auto Exchange::setOrderCallback(MatchingEngine::OrderCb cb) -> void { mOrderCallback = std::move(cb); }

auto Exchange::setTradeCallback(MatchingEngine::TradeCb cb) -> void { mTradeCallback = std::move(cb); }

auto Exchange::processRequest(const common::OrderRequest &request, const std::optional<std::string> &id) -> void {
  // Retrieve/Create MatchingEngine for the symbol
  auto &engine = getOrCreateEngine(request.symbol);

  // Given ID means we will have a REMOVE or MODIFY
  if (id.has_value()) {
    (request.qty == 0) ? engine.cancelOrder(*id) : engine.modifyOrder(*id, request);
  } else {
    // New Order
    auto newId = "ORDER" + std::to_string(mOrderId++);
    engine.addOrder(newId, request);
  }
}

auto Exchange::getOrCreateEngine(const std::string &symbol) -> MatchingEngine & {
  if (auto it = mEngines.find(symbol); it != mEngines.end()) {
    return it->second; // return existing engine
  }

  // Create and return new engine
  auto [newIt, _] = mEngines.emplace(symbol, MatchingEngine(symbol, mOrderCallback, mTradeCallback));
  return newIt->second;
}

} // namespace exchange
