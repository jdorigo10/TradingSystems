#include "trade/PositionTracker.hpp"

namespace trade {

auto PositionTracker::onTradeEvent(const std::string &orderId, const common::TradeEvent &event) -> void {
  if (orderId == event.sellSideId) {
    // Modify the SELL side order
    handleSell(event.symbol, event.qty, event.price);
  }

  if (orderId == event.buySideId) {
    // Modify the BUY side order
    handleBuy(event.symbol, event.qty, event.price);
  }
}

auto PositionTracker::updateMarketPrice(const std::string &symbol, double price) -> void {
  mPositions[symbol].lastMark = price;
}

// Gets the our Position for the given Symbol
auto PositionTracker::getPosition(const std::string &symbol) const -> std::optional<common::Position> {
  auto it = mPositions.find(symbol);
  if (it == mPositions.end()) {
    return std::nullopt;
  }

  return it->second;
}

void PositionTracker::handleBuy(const std::string &symbol, int qty, double price) {
  auto &position = mPositions[symbol];

  // Closing short position
  if (position.position < 0) {
    // amount used to close the short position
    int closingQty = std::min(qty, -position.position);

    // For shorts...
    // Profit = entry price (avg) - buy price
    // PnL = profit * amount bought to close short
    position.realizedPnL += (position.avgPrice - price) * closingQty;

    // Add to position to reduce the short
    position.position += closingQty;
    qty -= closingQty;

    if (position.position == 0) {
      // reset avg entry price when flat (no longer shorting)
      position.avgPrice = 0;
    }
  }

  // Remaining qty opens/increases long position
  if (qty > 0) {
    double totalCost = (position.avgPrice * position.position) + (price * qty);

    // Add to position to increase the long
    position.position += qty;

    // find avg entry price for long position
    position.avgPrice = totalCost / position.position;
  }

  position.print(symbol);
}

void PositionTracker::handleSell(const std::string &symbol, int qty, double price) {
  auto &position = mPositions[symbol];

  // Closing long position
  if (position.position > 0) {
    // amount used to close the long position
    int closingQty = std::min(qty, position.position);

    // For longs...
    // Profit = sell price - entry price (avg)
    // PnL = profit * amount sold to close long
    position.realizedPnL += (price - position.avgPrice) * closingQty;

    // Decrease from position to reduce the long
    position.position -= closingQty;
    qty -= closingQty;

    if (position.position == 0) {
      // reset avg entry price when flat (no longer longing)
      position.avgPrice = 0;
    }
  }

  // Remaining qty opens/increases short position
  if (qty > 0) {
    double totalCost = (position.avgPrice * (-position.position)) + (price * qty);

    // Decrease from position to increase the short
    position.position -= qty;

    // find avg entry price for short position
    position.avgPrice = totalCost / (-position.position);
  }

  position.print(symbol);
}

} // namespace trade
