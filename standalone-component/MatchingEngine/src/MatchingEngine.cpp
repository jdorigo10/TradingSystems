#include <iostream>

#include "MatchingEngine.hpp"

auto MatchingEngine::submitBuyOrder(int id, int price, int qty) -> void {
  // Run until fulfilled or no more matches
  while (qty > 0 && !mSellBook.empty()) {
    auto bestSellLevel = mSellBook.begin();
    int bestSellPrice = bestSellLevel->first;

    // Check if we have price match
    if (bestSellPrice > price) {
      break;
    }

    // Get our list of best sell orders to make trades
    auto &sellOrders = bestSellLevel->second;

    // Get our first best sell order to trade
    auto sellOrder = sellOrders.begin();
    int tradeQty = std::min(qty, sellOrder->quantity);

    // TRADE
    std::cout << "Trade Executed \n"
              << " (" << id << ") BUY " << price << "\n"
              << " (" << sellOrder->id << ") SELL " << sellOrder->price << "\n"
              << " QTY " << tradeQty << "\n"
              << std::endl;

    // Update quantities after trade
    qty = qty - tradeQty;
    sellOrder->quantity = sellOrder->quantity - tradeQty;

    // Remove sell order if fulfilled
    if (sellOrder->quantity == 0) {
      mOrderIndex.erase(sellOrder->id);
      sellOrders.erase(sellOrder);

      // Remove price level from Sell Book if emptied
      if (sellOrders.empty()) {
        mSellBook.erase(bestSellLevel);
      }
    }
  }

  // Add to order book if not fulfilled
  if (qty > 0) {
    addOrderToBook(id, price, qty, true);
  }
}

// Submits a SELL order
auto MatchingEngine::submitSellOrder(int id, int price, int qty) -> void {
  // Run until fulfilled or no more matches
  while (qty > 0 && !mBuyBook.empty()) {
    auto bestBuyLevel = mBuyBook.begin();
    int bestBuyPrice = bestBuyLevel->first;

    // Check if we have price match
    if (bestBuyPrice < price) {
      break;
    }

    // Get our list of best buy orders to make trades
    auto &buyOrders = bestBuyLevel->second;

    // Get our first best buy order to trade
    auto buyOrder = buyOrders.begin();
    int tradeQty = std::min(qty, buyOrder->quantity);

    // TRADE
    std::cout << "Trade Executed \n"
              << " (" << id << ") SELL " << price << "\n"
              << " (" << buyOrder->id << ") BUY " << buyOrder->price << "\n"
              << " QTY " << tradeQty << "\n"
              << std::endl;

    // Update quantities after trade
    qty = qty - tradeQty;
    buyOrder->quantity = buyOrder->quantity - tradeQty;

    // Remove buy order if fulfilled
    if (buyOrder->quantity == 0) {
      mOrderIndex.erase(buyOrder->id);
      buyOrders.erase(buyOrder);

      // Remove price level from Buy Book if emptied
      if (buyOrders.empty()) {
        mBuyBook.erase(bestBuyLevel);
      }
    }
  }

  // Add to order book if not fulfilled
  if (qty > 0) {
    addOrderToBook(id, price, qty, false);
  }
}

// CANCELs an order
auto MatchingEngine::cancel(int id) -> void {
  // Ensure the order exists
  if (!mOrderIndex.count(id)) {
    return;
  }

  // Get the Order
  auto order = mOrderIndex[id];
  auto price = order->price;

  if (order->isBuy) {
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

// Adds Order to the appropriate book
auto MatchingEngine::addOrderToBook(int id, int price, int qty, bool isBuy) -> void {
  Order order{id, price, qty, isBuy};

  if (isBuy) {
    // Add new order to Buy Book at price level
    auto &priceLevel = mBuyBook[price];
    priceLevel.push_back(order);

    // Get pointer to newly added order and add to order index
    auto it = prev(priceLevel.end());
    mOrderIndex[id] = it;
  } else {
    // Add new order to Sell Book at price level
    auto &priceLevel = mSellBook[price];
    priceLevel.push_back(order);

    // Get pointer to newly added order and add to order index
    auto it = prev(priceLevel.end());
    mOrderIndex[id] = it;
  }
}
