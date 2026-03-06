#include <iostream>
#include <string>
#include <iomanip>

/* Represents the position state of a trader (for one symbol)
 * - for more symbols, simply use something like
 *   unordered_map<string, PositionTracker> symbolPositions;
 * 
 * Tracks:
 * - position: net shares currently held
 * - avgPrice: average cost basis of the position
 * - realizedPnL: profit/loss from closed trades
 * - lastMark: most recent market price (for unrealized PnL)
*/
class PositionTracker {
    public:
        // Handles a trade execution
        void processTrade(const std::string& side, int qty, double price) {
            if (side == "BUY") {
                // BUY increases position
                handleBuy(qty, price);
            }
            else if (side == "SELL") {
                // SELL decreases position
                handleSell(qty, price);
            }
        }

        // Updates the latest market price
        void updateMark(double price) {
            mLastMark = price;
        }

        // Print current status
        void printStatus() {
            // Keep everything at 2 decimal points
            std::cout << std::fixed << std::setprecision(2);

            std::cout << "Position: " << mPosition << "\n"
                      << "Avg Price: " << mAvgPrice << "\n"

                      << "Total PnL: " << mRealizedPnL + unrealizedPnL() << "\n"
                      << " Realized PnL: " << mRealizedPnL << "\n"
                      << " Unrealized PnL: " << unrealizedPnL() << "\n"

                      << std::endl;
        }

    private: 
        // BUYing shares
        void handleBuy(int qty, double price) {
            // Closing short position
            if (mPosition < 0) {
                // amount used to close the short position
                int closingQty = std::min(qty, -mPosition);

                // For shorts..
                // Profit = entry price (avg) - buy price
                // PnL = profit * amount bought to close short
                mRealizedPnL += (mAvgPrice - price) * closingQty;

                // Add to position to reduce the short
                mPosition += closingQty;
                qty -= closingQty;

                if (mPosition == 0) {
                    // reset avg entry price when flat (no longer shorting)
                    mAvgPrice = 0;
                }
            }

            // Remaining qty opens/increases long position
            if (qty > 0) {
                double totalCost = (mAvgPrice * mPosition) + (price * qty);
                
                // Add to position to increase the long
                mPosition += qty;

                // find avg entry price for long position
                mAvgPrice = totalCost / mPosition;
            }
        }

        // SELLing shares
        void handleSell(int qty, double price) {
            // Closing long position
            if (mPosition > 0) {
                // amount used to close the long position
                int closingQty = std::min(qty, mPosition);

                // For longs...
                // Profit = sell price - entry price (avg)
                // PnL = profit * amount sold to close long
                mRealizedPnL += (price - mAvgPrice) * closingQty;

                // Decrease from position to reduce the long
                mPosition -= closingQty;
                qty -= closingQty;

                if (mPosition == 0) {
                    // reset avg entry price when flat (no longer longing)
                    mAvgPrice = 0;
                }
            }

            // Remaining qty opens/increases short position
            if (qty > 0) {
                double totalCost = (mAvgPrice * (-mPosition)) + (price * qty);

                // Decrease from position to increase the short
                mPosition -= qty;

                // find avg entry price for short position
                mAvgPrice = totalCost / (-mPosition);
            }
        }

        // Calculates unrealized PnL
        double unrealizedPnL() const {
            if (mPosition == 0) {
                // unrealized PnL is 0 for no position
                return 0.0;
            }

            // LONG POSITION
            // profit if market price increases
            if (mPosition > 0)
            {
                return (mLastMark - mAvgPrice) * mPosition;
            }

            // SHORT POSITION
            // profit if market price decreases
            else
            {
                return (mAvgPrice - mLastMark) * (-mPosition);
            }
        }

        int mPosition = 0;          // net shares (+ long, - short)
        double mAvgPrice = 0.0;     // average entry price
        double mRealizedPnL = 0.0;  // realized Profit & Loss from closed trades
        double mLastMark = 0.0;     // latest market price
};


int main()
{
    PositionTracker tracker;

    std::string line;

    // Read commands from stdin until EOF.
    while (getline(std::cin, line)) {
        if (line.empty()) {
			continue;
		}

        std::stringstream ss(line);

        std::string cmd;
        ss >> cmd;

        if (cmd == "TRADE")
        {
            std::string side;
            int qty;
            double price;
            ss >> side >> qty >> price;

            tracker.processTrade(side, qty, price);
        }
        else if (cmd == "MARK")
        {
            double price;
            ss >> price;

            tracker.updateMark(price);
        }
        else if (cmd == "PRINT")
        {
            tracker.printStatus();
        }
        else
        {
            std::cout << "Unknown command\n";
        }
    }

    return 0;
}