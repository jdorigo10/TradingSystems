#include <functional>
#include <list>
#include <map>
#include <unordered_map>

#include "common/Order.hpp"
#include "common/OrderRequest.hpp"

#ifndef EXCHANGE_MATCHING_ENGINE_HPP
#define EXCHANGE_MATCHING_ENGINE_HPP

namespace exchange {

/* Engine that accepts orders: BUY, SELL, CANCEL
 * - matches orders based on best price
 * - First In First Out for same price
 *
 * Actions
 * - BUY: highest price first, will match with lowest sell that satisfies
 * - SELL: lowest price first, will match wil highest buy that satisfies
 * - CANCEL: will remove order from books
 */
class MatchingEngine {
public:
  using OrderCb =
      std::function<void(const std::string &id, const std::string &symbol, const common::OrderAction &action,
                         const common::OrderSide &side, double price, int qty, bool isUserOrder, bool isFilled)>;
  using TradeCb = std::function<void(const std::string &symbol, double price, int qty, const std::string &sellSideId,
                                     const std::string &buySideId)>;

  explicit MatchingEngine(std::string symbol, OrderCb orderCb, TradeCb tradeCb)
      : mSymbol(std::move(symbol)), mOrderCb(std::move(orderCb)), mTradeCb(std::move(tradeCb)) {}

  // Handles adding of a new Order given an ID
  auto addOrder(const std::string &id, const common::OrderRequest &request, bool isModify = false) -> void;

  // Handles removing of an existing Order given an ID
  auto cancelOrder(const std::string &id, bool isModify = false) -> void;

  // Handles modifying of an existing Order given an ID (cancel -> add)
  auto modifyOrder(const std::string &id, const common::OrderRequest &request) -> void;

private:
  // Matches an incoming BUY order to an existing SELL one for a Trade
  auto matchBuyOrder(common::Order &order) -> bool;

  // Matches an incoming SELL order to an existing BUY one for a Trade
  auto matchSellOrder(common::Order &order) -> bool;

  // Adds Order to the appropriate book
  auto addToBook(common::Order order) -> void;

  // Helper to fire an ADD msg to the feed
  auto fireAdd(const common::Order &order, bool isUserOrder) -> void;

  // Helper to fire a MODIFY msg to the feed
  auto fireModify(const common::Order &order) -> void;

  // Helper to fire a REMOVE msg to the feed
  auto fireRemove(const common::Order &order, bool isFilled = false) -> void;

  // Helper to fire a TRADE msg to the feed
  auto fireTrade(const std::string &sellSideId, const std::string &buySideId, double price, int qty) -> void;

  // Symbol for the Book
  std::string mSymbol;

  // Callbacks for an raw Order Update msg or raw Trade Event msg
  OrderCb mOrderCb;
  TradeCb mTradeCb;

  // Maps: OrderId -> iterator pointing to the order inside a list
  std::unordered_map<std::string, std::list<common::Order>::iterator> mOrderIndex;

  // Map of Prices -> FIFO list of BUY Orders (lowest price first)
  std::map<double, std::list<common::Order>, std::greater<>> mBuyBook;

  // Map of Prices -> FIFO list of SELL Orders (highest price first)
  std::map<double, std::list<common::Order>, std::less<>> mSellBook;
};

} // namespace exchange

#endif
