#include "mp/BookManager.hpp"

namespace mp {

auto OrderBookManager::onOrderUpdate(const common::OrderUpdate &update) -> void {
  auto it = mBooks.find(update.symbol);

  if (it == mBooks.end()) {
    // create book lazily
    auto book = std::make_unique<OrderBook>();
    it = mBooks.emplace(update.symbol, std::move(book)).first;
  }

  it->second->onOrderUpdate(update);

  snapshot(update.symbol);
}

auto OrderBookManager::getBestBid(const std::string &symbol) const -> std::optional<common::Order> {
  if (auto it = mBooks.find(symbol); it != mBooks.end()) {
    return it->second->getBestBid();
  }

  return std::nullopt;
}

auto OrderBookManager::getBestAsk(const std::string &symbol) const -> std::optional<common::Order> {
  if (auto it = mBooks.find(symbol); it != mBooks.end()) {
    return it->second->getBestAsk();
  }

  return std::nullopt;
}

auto OrderBookManager::snapshot(const std::string &symbol) -> void {
  if (auto it = mBooks.find(symbol); it != mBooks.end()) {
    std::cout << it->first << " Book\n";
    return it->second->snapshot();
  }
}

} // namespace mp