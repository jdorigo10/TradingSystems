#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>

// Represents the lifecycle state of an order
enum class OrderStatus {
    NEW,
    PARTIALLY_FILLED,
    FILLED,
    CANCELLED
};

// Represents a single order
struct Order {
    std::string id;      // Unique Order ID
    std::string symbol;  // symbol: "AAPL"
    std::string side;    // BUY or SELL
    int quantity;        // Original Order quantity
    int filled = 0;      // Quantity filled so far
    double price;        // Limit price
    OrderStatus status;  // Current state of the order

    void print() const {
        std::cout << "Order " << id
                  << " [" << symbol << "] "
                  << side
                  << " price=" << price
                  << " qty=" << quantity
                  << " filled=" << filled
                  << " status=";
        switch (status) {
            case OrderStatus::NEW: std::cout << "NEW"; break;
            case OrderStatus::PARTIALLY_FILLED: std::cout << "PARTIALLY_FILLED"; break;
            case OrderStatus::FILLED: std::cout << "FILLED"; break;
            case OrderStatus::CANCELLED: std::cout << "CANCELLED"; break;
        }
        std::cout << std::endl;
    }
};

/* Order Life Cycle Tracker
 * - tracks state of each order: NEW, PARTIALLY_FILLED, FILLED, CANCELLED
 * - track order details: ID, symbol, BUY/SELL, qty, filled qty, price
 * - allows updates for: FILL event, CANCEL event, NEW event
*/
class OrderTracker {
    public:
        // Creates a NEW order
        void newOrder(const std::string& id, const std::string& symbol, const std::string& side, int qty, double price) {
            Order newOrder{id, symbol, side, qty, 0, price, OrderStatus::NEW};
            mOrders[id] = newOrder;

            std::cout << "NEW order: ";
            newOrder.print();
        }

        // FILL an order (partially or full)
        void fillOrder(const std::string& id, int fillQty) {
            if (!mOrders.count(id)) {
                // Order does not exist
                return;
            }

            Order& order = mOrders[id];

            // Update filled qty
            order.filled += fillQty;

            if (order.filled >= order.quantity) {
                order.status = OrderStatus::FILLED;
                order.filled = order.quantity;  // cap it
            }
            else {
                order.status = OrderStatus::PARTIALLY_FILLED;
            }

            std::cout << "FILL order: ";
            order.print();
        }

        // CANCEL an order
        void cancelOrder(const std::string& id) {
            if (!mOrders.count(id)) {
                // Order does not exist
                return;
            }

            Order& order = mOrders[id];

            // Only allow cancelling if not fully filled or already cancelled
            if (order.status == OrderStatus::FILLED || order.status == OrderStatus::CANCELLED) {
                return;
            }

            order.status = OrderStatus::CANCELLED;

            std::cout << "CANCEL order: ";
            order.print();
        }

        // Prints all the orders being tracked
        void printAllOrders() const {
            std::cout << "\n+--------------- All Orders ---------------+\n";
            for (const auto& [id, order]: mOrders) {
                order.print();
            }
            std::cout << "\n";
        }

    private:
        // Maps: Order ID -> Order
        // - allows for O(1) lookup by order id
        std::unordered_map<std::string, Order> mOrders;
};


int main()
{
    OrderTracker tracker;

    std::string line;

    // Read commands from stdin until EOF.
    while (getline(std::cin, line)) {
        if (line.empty()) {
			continue;
		}

        std::stringstream ss(line);

        std::string cmd;
        ss >> cmd;

        if (cmd == "NEW")
        {
            std::string id, symbol, side;
            int qty;
            double price;
            ss >> id >> symbol >> side >> qty >> price;

            tracker.newOrder(id, symbol, side, qty, price);
        }
        else if (cmd == "FILL")
        {
            std::string id;
            int fillQty;
            ss >> id >> fillQty;

            tracker.fillOrder(id, fillQty);
        }
        else if (cmd == "CANCEL")
        {
            std::string id;
            ss >> id;

            tracker.cancelOrder(id);
        }
        else if (cmd == "PRINT")
        {
            tracker.printAllOrders();
        }
        else
        {
            std::cout << "Unknown command\n";
        }
    }

    return 0;
}