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

  snapshot();
}

auto OrderBookManager::snapshot() -> void {
  for (const auto &book : mBooks) {
    book.second->bookSnapshot(book.first);
  }
}

} // namespace mp