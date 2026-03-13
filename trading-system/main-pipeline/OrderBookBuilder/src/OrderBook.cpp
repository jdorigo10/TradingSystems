#include "mp/OrderBook.hpp"

namespace mp {

auto OrderBook::onOrderUpdate(const common::OrderUpdate &update) -> void {
  switch (update.type) {
  case common::OrderUpdate::Type::ADD:
    addOrder(update);
    break;

  case common::OrderUpdate::Type::MODIFY:
    modifyOrder(update);
    break;

  case common::OrderUpdate::Type::REMOVE:
    removeOrder(update);
    break;
  }
}

auto OrderBook::getBestBid() const -> std::optional<common::Order> {
  if (auto bidIt = mBids.begin(); bidIt != mBids.end()) {
    if (!bidIt->second.empty()) {
      auto order = bidIt->second.front();
      return *order;
    }
  }

  return std::nullopt;
}

auto OrderBook::getBestAsk() const -> std::optional<common::Order> {
  if (auto askIt = mAsks.begin(); askIt != mAsks.end()) {
    if (!askIt->second.empty()) {
      auto order = askIt->second.front();
      return *order;
    }
  }

  return std::nullopt;
}

auto OrderBook::snapshot() const -> void {
  std::cout << "  Best Bid: ";
  auto bestBid = getBestBid();
  if (bestBid.has_value()) {
    bestBid->print();
  }
  std::cout << "\n";

  std::cout << "  Best Ask: ";
  auto bestAsk = getBestAsk();
  if (bestAsk.has_value()) {
    bestAsk->print();
  }
  std::cout << "\n";

  std::cout << "\n";
}

auto OrderBook::addOrder(const common::OrderUpdate &update) -> void {
  common::Order order;
  order.id = update.id;
  order.side = (update.side == common::OrderUpdate::Side::BUY) ? common::Order::Side::BUY : common::Order::Side::SELL;
  order.price = update.price;
  order.qty = update.qty;

  // Get the correct Book based on the side (BUY/SELL)
  auto &bookSide = (order.side == common::Order::Side::BUY) ? mBids : mAsks;

  // Get/Create the Price Level in the book based on the orders price
  auto &priceLevel = bookSide[order.price];

  // Store the pointer to the Order in the Price Levels FIFO list
  priceLevel.push_back(&mOrderLookup[order.id]);

  // Store/Update the Order in its lookup map
  mOrderLookup[order.id] = order;

  // Store the Orders iterator from the FIFO list with it in lookup map
  mOrderLookup[order.id].listIt = std::prev(priceLevel.end());
}

auto OrderBook::modifyOrder(const common::OrderUpdate &update) -> void {
  // Retrieve order from the lookup map
  auto it = mOrderLookup.find(update.id);
  if (it == mOrderLookup.end()) {
    return;
  }
  common::Order &order = it->second;

  // Price Update
  if (order.price != update.price) {
    // Remove order from Price Level
    removeFromPriceLevel(order);

    // Update the order price
    order.price = update.price;

    // Get the correct Book based on the side (BUY/SELL)
    auto &bookSide = (order.side == common::Order::Side::BUY) ? mBids : mAsks;

    // Get/Create the Price Level in the book based on the new order price
    auto &priceLevel = bookSide[order.price];

    // Store the pointer to the Order in the new Price Levels FIFO list
    priceLevel.push_back(&order);

    // Store the Orders iterator from the new FIFO list with it in lookup map
    order.listIt = std::prev(priceLevel.end());
  }

  // Qunatity Update
  order.qty = update.qty;
}

auto OrderBook::removeOrder(const common::OrderUpdate &update) -> void {
  // Retrieve order from the lookup map
  auto it = mOrderLookup.find(update.id);
  if (it == mOrderLookup.end()) {
    return;
  }
  common::Order &order = it->second;

  // Remove order from Price Level
  removeFromPriceLevel(order);

  // Remove order from lookup map
  mOrderLookup.erase(it);
}

auto OrderBook::removeFromPriceLevel(common::Order &order) -> void {
  // Get the correct Book based on the side (BUY/SELL)
  auto &bookSide = (order.side == common::Order::Side::BUY) ? mBids : mAsks;

  // Retrieve the Price Level FIFO list for the given orders price
  auto priceLevelIt = bookSide.find(order.price);
  if (priceLevelIt == bookSide.end()) {
    return;
  }
  auto &priceLevel = priceLevelIt->second;

  // Remove the order from the price level
  priceLevel.erase(order.listIt);

  // Remove the Price Level itself from the book if now empty
  if (priceLevel.empty()) {
    bookSide.erase(priceLevelIt);
  }
}

} // namespace mp
