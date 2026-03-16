
#include "trade/OrderTracker.hpp"

namespace trade {

auto OrderTracker::onOrderUpdate(const common::OrderUpdate &update) -> void {
  switch (update.type) {
  case common::OrderUpdateType::ADD:
    addOrder(update);
    break;

  case common::OrderUpdateType::MODIFY:
    modifyOrder(update.id, update.qty);
    break;

  case common::OrderUpdateType::REMOVE:
    removeOrder(update.id);
    break;
  }
}

auto OrderTracker::getOrderFromTrade(const common::TradeEvent &event) const -> std::optional<common::LiveOrder> {
  auto sellIt = mOrders.find(event.sellSideId);
  if (sellIt != mOrders.end()) {
    return sellIt->second;
  }

  auto buyIt = mOrders.find(event.buySideId);
  if (buyIt != mOrders.end()) {
    return buyIt->second;
  }

  return std::nullopt;
}

auto OrderTracker::addOrder(const common::OrderUpdate &update) -> void {
  if (!update.isUserOrder) {
    return; // not ours
  }

  common::LiveOrder newOrder;
  newOrder.id = update.id;
  newOrder.symbol = update.symbol;
  newOrder.side = update.side;

  newOrder.qty = update.qty;
  newOrder.price = update.price;
  newOrder.status = common::LiveOrderStatus::NEW;

  mOrders[newOrder.id] = newOrder;

  std::cout << "NEW order: ";
  newOrder.print();
}

auto OrderTracker::modifyOrder(const std::string &id, int qty) -> void {
  if (!mOrders.count(id)) {
    // Order does not exist (is not ours)
    return;
  }
  common::LiveOrder &order = mOrders[id];

  // Update the QTY
  order.filled = order.qty - qty;

  if (order.filled >= order.qty) {
    order.status = common::LiveOrderStatus::FILLED;
    order.filled = order.qty; // cap it
  } else {
    order.status = common::LiveOrderStatus::PARTIALLY_FILLED;
  }

  std::cout << "FILL order: ";
  order.print();
}

auto OrderTracker::removeOrder(const std::string &id) -> void {
  if (!mOrders.count(id)) {
    // Order does not exist (is not ours)
    return;
  }
  common::LiveOrder &order = mOrders[id];

  // Only allow cancelling if not fully filled or already cancelled
  if (order.status == common::LiveOrderStatus::FILLED || order.status == common::LiveOrderStatus::CANCELLED) {
    return;
  }

  order.status = common::LiveOrderStatus::CANCELLED;

  std::cout << "CANCEL order: ";
  order.print();
}

} // namespace trade
