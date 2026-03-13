#include "mp/BookManager.hpp"

namespace mp {

auto OrderBookManager::onOrderUpdate(const common::OrderUpdate &update) -> void {
  auto it = mBooks.find(update.symbol);
  if (it == mBooks.end()) {
    // create book lazily
    OrderBook book{};
    it = mBooks.emplace(update.symbol, std::move(book)).first;
  }

  it->second.onOrderUpdate(update);

  snapshot(update.symbol);
}

auto OrderBookManager::getBook(const std::string &symbol) -> std::optional<OrderBook> {
  auto it = mBooks.find(symbol);
  if (it == mBooks.end()) {
    return std::nullopt;
  }

  return it->second;
}

auto OrderBookManager::snapshot(const std::string &symbol) -> void {
  if (auto it = mBooks.find(symbol); it != mBooks.end()) {
    std::cout << it->first << " Book\n";
    return it->second.snapshot();
  }
}

} // namespace mp