#include <optional>
#include <string>
#include <unordered_map>

#include "common/LiveOrder.hpp"
#include "common/Position.hpp"
#include "common/TradeEvent.hpp"

#ifndef TRADE_POSITION_TRACKER_HPP
#define TRADE_POSITION_TRACKER_HPP

namespace trade {

/* PositionTracker
 * - maintains OUR position per symbol, calculates PnLs
 */
class PositionTracker {
public:
  // Handles a TradeEvent
  auto onTradeEvent(const std::string &orderId, const common::TradeEvent &event) -> void;

  // Updates the latest markey price for a given Symbol
  auto updateMarketPrice(const std::string &symbol, double price) -> void;

  // Gets the our Position for the given Symbol
  auto getPosition(const std::string &symbol) const -> std::optional<common::Position>;

private:
  // Helper to handle an executed BUY order
  auto handleBuy(const std::string &symbol, int qty, double price) -> void;

  // Helper to handle am executed SELL order
  auto handleSell(const std::string &symbol, int qty, double price) -> void;

  // Map of Symbol -> Position
  std::unordered_map<std::string, common::Position> mPositions;
};
} // namespace trade

#endif
