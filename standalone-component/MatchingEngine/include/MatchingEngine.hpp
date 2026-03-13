#include <list>
#include <map>
#include <unordered_map>

#ifndef MATCHIN_ENGINE_HPP
#define MATCHING_ENGINE_HPP

/* Represents a single Order
 *
 * Why a struct?
 * - lightweight data container
 * - frequently accessed in hot path
 */
struct Order {
  int id;       // unique order ID
  int price;    // limit price
  int quantity; // remaining quantity
  bool isBuy;   // buy or sell
};

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
  // Submits a BUY order
  auto submitBuyOrder(int id, int price, int qty) -> void;

  // Submits a SELL order
  auto submitSellOrder(int id, int price, int qty) -> void;

  // CANCELs an order
  auto cancel(int id) -> void;

private:
  // Adds Order to the appropriate book
  auto addOrderToBook(int id, int price, int qty, bool isBuy) -> void;

  /* Maps: OrderId -> iterator pointing to the order inside a list
   * - allows fast lookup of orders by ID
   *
   * Why unordered_map?
   * - allows O(1) lookup and cancels
   * - order IDs are unique keys
   */
  std::unordered_map<int, std::list<Order>::iterator> mOrderIndex;

  /* Maps: Price (high to low) -> FIFO queue of BUY orders
   *
   * Why map?
   * - begin() always gives the highest price (best bid)
   */
  std::map<int, std::list<Order>, std::greater<int>> mBuyBook;

  /* Maps: Price (low to high) -> FIFO queue of SELL orders
   *
   * Why map?
   * - begin() always gives the lowest price (best ask)
   */
  std::map<int, std::list<Order>> mSellBook;
};

#endif
