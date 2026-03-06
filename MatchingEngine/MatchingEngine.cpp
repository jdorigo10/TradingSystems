#include <iostream>
#include <map>
#include <unordered_map>
#include <list>
#include <memory>
#include <climits>
#include <sstream>
#include <string>

using namespace std;

/* Represents a single Order
 *
 * Why a struct?
 * - lightweight data container
 * - frequently accessed in hot path
 */
struct Order {
	int id;        // unique order ID
	int price;     // limit price
	int quantity;  // remaining quantity
	bool isBuy;    // buy or sell
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
		void submitBuyOrder(int id, int price, int qty) {
			// Run until fulfilled or no more matches
			while (qty > 0 && !mSellBook.empty()) {
				auto bestSellLevel = mSellBook.begin();
				int bestSellPrice = bestSellLevel->first;

				// Check if we have price match
				if (bestSellPrice > price) {
					break;
				}

				// Get our list of best sell orders to make trades
				auto& sellOrders = bestSellLevel->second;

				// Get our first best sell order to trade
				auto sellOrder = sellOrders.begin();
				int tradeQty = min(qty, sellOrder->quantity);

				// TRADE
				cout << "Trade Executed \n"
				     << " (" << id << ") BUY " << price << "\n" 
					 << " (" << sellOrder->id << ") SELL " << sellOrder->price << "\n"
					 << " QTY " << tradeQty << "\n" << endl;

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
		void submitSellOrder(int id, int price, int qty) {
			// Run until fulfilled or no more matches
			while (qty > 0 && !mBuyBook.empty()) {
				auto bestBuyLevel = mBuyBook.begin();
				int bestBuyPrice = bestBuyLevel->first;

				// Check if we have price match
				if (bestBuyPrice < price) {
					break;
				}

				// Get our list of best buy orders to make trades
				auto& buyOrders = bestBuyLevel->second;

				// Get our first best buy order to trade
				auto buyOrder = buyOrders.begin();
				int tradeQty = min(qty, buyOrder->quantity);

				// TRADE
				cout << "Trade Executed \n"
				     << " (" << id << ") SELL " << price << "\n" 
					 << " (" << buyOrder->id << ") BUY " << buyOrder->price << "\n"
					 << " QTY " << tradeQty << "\n" << endl;

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
		void cancel(int id) {
			// Ensure the order exists
			if (!mOrderIndex.count(id)) {
				return;
			}

			// Get the Order
			auto order = mOrderIndex[id];
			auto price = order->price;

			if (order->isBuy) {
				// Get our list of buy orders at this price
				auto& buyOrders = mBuyBook[price];

				// Remove buy order
				buyOrders.erase(order);

				// Remove price level from Buy Book if emptied
				if (buyOrders.empty()) {
					mBuyBook.erase(price);
				}
			}
			else {
				// Get our list of sell orders at this price
				auto& sellOrders = mSellBook[price];

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

	private:
		// Adds Order to the appropriate book
		void addOrderToBook(int id, int price, int qty, bool isBuy) {
			Order order{id, price, qty, isBuy};

			if (isBuy) {
				// Add new order to Buy Book at price level
				auto& priceLevel = mBuyBook[price];
				priceLevel.push_back(order);

				// Get pointer to newly added order and add to order index
				auto it = prev(priceLevel.end());
				mOrderIndex[id] = it;
			}
			else {
				// Add new order to Sell Book at price level
				auto& priceLevel = mSellBook[price];
				priceLevel.push_back(order);

				// Get pointer to newly added order and add to order index
				auto it = prev(priceLevel.end());
				mOrderIndex[id] = it;
			}
		}

		/*******************
		 ***   STORAGE   ***
		 *******************/

		/* Maps: OrderId -> iterator pointing to the order inside a list
		 * - allows fast lookup of orders by ID
		 *
		 * Why unordered_map?
		 * - allows O(1) lookup and cancels
		 * - order IDs are unique keys
		 */
		unordered_map<int, list<Order>::iterator> mOrderIndex;

		/* Maps: Price (high to low) -> FIFO queue of BUY orders
		 * 
		 * Why map?
		 * - begin() always gives the highest price (best bid)
		 */
		map<int, list<Order>, greater<int>> mBuyBook;

		/* Maps: Price (low to high) -> FIFO queue of SELL orders
		 * 
		 * Why map?
		 * - begin() always gives the lowest price (best ask)
		 */
		map<int, list<Order>> mSellBook;
};


int main()
{
    MatchingEngine engine;

    string line;

    // Read commands from stdin until EOF.
    while (getline(cin, line)) {
        if (line.empty()) {
			continue;
		}

        stringstream ss(line);

        int id;
        string cmd;
        ss >> id >> cmd;

        if (cmd == "BUY")
        {
            int price, qty;
            ss >> price >> qty;

            engine.submitBuyOrder(id, price, qty);
        }
        else if (cmd == "SELL")
        {
            int price, qty;
            ss >> price >> qty;

            engine.submitSellOrder(id, price, qty);
        }
        else if (cmd == "CANCEL")
        {
            engine.cancel(id);
        }
        else
        {
            cout << "Unknown command\n";
        }
    }

    return 0;
}
