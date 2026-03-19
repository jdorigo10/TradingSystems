#include <iostream>

#include "exchange/MatchingEngine.hpp"

namespace exchange {

auto MatchingEngine::addOrder(const std::string &id, const common::OrderRequest &request, bool isModify) -> void {
  common::Order order{id, request.side, request.price, request.qty};

  // Send ADD/MODIFY msg to feed
  (!isModify) ? fireAdd(order, request.isUserOrder) : fireModify(order);

  // Match request for a Trade
  auto traded = (order.side == common::OrderSide::BUY) ? matchBuyOrder(order) : matchSellOrder(order);

  // Add to order book if not fulfilled
  if (order.qty > 0) {
    // Send MODIFY msg to feed (if a trade occurred)
    if (traded) {
      fireModify(order);
    }

    addToBook(std::move(order));
  } else {
    // Send REMOVE msg to feed (flagged as a modify, not cancel)
    fireRemove(order, true);
  }
}

auto MatchingEngine::cancelOrder(const std::string &id, bool isModify) -> void {
  // Ensure the order exists
  if (!mOrderIndex.count(id)) {
    return;
  }

  // Get the Order
  auto order = mOrderIndex[id];
  auto price = order->price;

  // Send REMOVE msg to feed (ignore on modify requests)
  if (!isModify) {
    fireRemove(*order);
  }

  if (order->side == common::OrderSide::BUY) {
    // Get our list of buy orders at this price
    auto &buyOrders = mBuyBook[price];

    // Remove buy order
    buyOrders.erase(order);

    // Remove price level from Buy Book if emptied
    if (buyOrders.empty()) {
      mBuyBook.erase(price);
    }
  } else {
    // Get our list of sell orders at this price
    auto &sellOrders = mSellBook[price];

    // Remove sell order
    sellOrders.erase(order);

    // Remove price level from Sell Book if emptied
    if (sellOrders.empty()) {
      mSellBook.erase(price);
    }
  }

  // Remove order from Order Index
  mOrderIndex.erase(id);
}

auto MatchingEngine::modifyOrder(const std::string &id, const common::OrderRequest &request) -> void {
  // Cancel the existing order (noting its a modify request)
  cancelOrder(id, true);

  // Add the new modified version of the order (noting its a modify request)
  addOrder(id, request, true);
}

auto MatchingEngine::matchBuyOrder(common::Order &order) -> bool {
  bool traded = false;

  // Run until fulfilled or no more matches
  while (order.qty > 0 && !mSellBook.empty()) {
    auto bestSellLevel = mSellBook.begin();
    double bestSellPrice = bestSellLevel->first;

    // Check if we have price match
    if (bestSellPrice > order.price) {
      break;
    }

    // Get our list of best sell orders to make trades
    auto &sellOrders = bestSellLevel->second;

    // Get our first best sell order to trade
    auto sellOrder = sellOrders.begin();
    int tradeQty = std::min(order.qty, sellOrder->qty);

    // Send TRADE msg to feed
    fireTrade(sellOrder->id, order.id, bestSellPrice, tradeQty);
    traded = true;

    // Update quantities after trade
    order.qty = order.qty - tradeQty;
    sellOrder->qty = sellOrder->qty - tradeQty;

    // Remove sell order if fulfilled
    if (sellOrder->qty == 0) {
      // Send REMOVE msg to feed (flagged as a modify, not cancel)
      fireRemove(*sellOrder, true);

      mOrderIndex.erase(sellOrder->id);
      sellOrders.erase(sellOrder);

      // Remove price level from Sell Book if emptied
      if (sellOrders.empty()) {
        mSellBook.erase(bestSellLevel);
      }
    } else {
      // Send MODIFY msg to feed
      fireModify(*sellOrder);
    }
  }

  return traded;
}

auto MatchingEngine::matchSellOrder(common::Order &order) -> bool {
  bool traded = false;

  // Run until fulfilled or no more matches
  while (order.qty > 0 && !mBuyBook.empty()) {
    auto bestBuyLevel = mBuyBook.begin();
    double bestBuyPrice = bestBuyLevel->first;

    // Check if we have price match
    if (bestBuyPrice < order.price) {
      break;
    }

    // Get our list of best buy orders to make trades
    auto &buyOrders = bestBuyLevel->second;

    // Get our first best buy order to trade
    auto buyOrder = buyOrders.begin();
    int tradeQty = std::min(order.qty, buyOrder->qty);

    // TRADE
    fireTrade(buyOrder->id, order.id, bestBuyPrice, tradeQty);
    traded = true;

    // Update quantities after trade
    order.qty = order.qty - tradeQty;
    buyOrder->qty = buyOrder->qty - tradeQty;

    // Remove buy order if fulfilled
    if (buyOrder->qty == 0) {
      // Send REMOVE msg to feed (flagged as a modify, not cancel)
      fireRemove(*buyOrder, true);

      mOrderIndex.erase(buyOrder->id);
      buyOrders.erase(buyOrder);

      // Remove price level from Buy Book if emptied
      if (buyOrders.empty()) {
        mBuyBook.erase(bestBuyLevel);
      }
    } else {
      // Send MODIFY msg to feed
      fireModify(*buyOrder);
    }
  }

  return traded;
}

auto MatchingEngine::addToBook(common::Order order) -> void {
  auto id = order.id;

  // Get the price level from the correct book
  auto &priceLevel = (order.side == common::OrderSide::BUY) ? mBuyBook[order.price] : mSellBook[order.price];

  // Add new order to the Book at price level
  priceLevel.push_back(std::move(order));

  // Get pointer to newly added order and add to order index
  auto it = prev(priceLevel.end());
  mOrderIndex[id] = it;
}

auto MatchingEngine::fireAdd(const common::Order &order, bool isUserOrder) -> void {
  if (mOrderCb) {
    mOrderCb(order.id, mSymbol, common::OrderAction::ADD, order.side, order.price, order.qty, isUserOrder, false);
  }
}

auto MatchingEngine::fireModify(const common::Order &order) -> void {
  if (mOrderCb) {
    // Disregards: isUser
    mOrderCb(order.id, mSymbol, common::OrderAction::MODIFY, order.side, order.price, order.qty, false, false);
  }
}

auto MatchingEngine::fireRemove(const common::Order &order, bool isFilled) -> void {
  if (mOrderCb) {
    // Disregards: side, price, qty, isUser
    mOrderCb(order.id, mSymbol, common::OrderAction::REMOVE, order.side, order.price, order.qty, false, isFilled);
  }
}

auto MatchingEngine::fireTrade(const std::string &sellSideId, const std::string &buySideId, double price, int qty)
    -> void {
  if (mTradeCb) {
    mTradeCb(mSymbol, price, qty, sellSideId, buySideId);
  }
}

} // namespace exchange
