#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

// Represents a Price Level
struct PriceLevel {
    int qty;
    double price;
};

// Represents a Trade
struct Trade {
    int qty;
    double risk;

    bool isHedge;
};

/* RiskHandler
 * - will process the trade (execute it against the book)
 * - will then handle how much it needs to hedge that trade based on the risk
 *   (most likely process a hedge trade)
 * 
 * Assumption: There will always be enough QTY in boook to execute trade
*/
class RiskHandler {
    public:
        explicit RiskHandler(std::vector<PriceLevel> buyPrices, std::vector<PriceLevel> sellPrices) : mBuyPrices(std::move(buyPrices)), mSellPrices(std::move(sellPrices)) {}

        // Process a Trade and any Hedge trades needed for the risk
        void processTrade(Trade trade) {
            // Ignore trades of 0 qty
            if (trade.qty == 0) {
                return;
            }

            // Determine HedgeQty and at to our rolling risk
            mRollingRisk += -(trade.qty * trade.risk);

            // Execute trade
            if (trade.qty > 0) {
                executeBuy(trade);
            }
            else {
                executeSell(trade);
            }

            // Create Hedge Trade based on current rolling risk (whole numbers only)
            int hedgeQty = static_cast<int>(mRollingRisk);
            mRollingRisk -= hedgeQty;
            Trade hedgeTrade{hedgeQty, 0, true};

            // Execute Hedge Trade
            if (hedgeTrade.qty > 0) {
                executeBuy(hedgeTrade);
            }
            else if (hedgeTrade.qty < 0) {
                executeSell(hedgeTrade);
            }
        }

    private:
        // Executes a BUY on the book
        void executeBuy(Trade& trade) {
            int qtyTraded = 0;
            double priceTraded = 0;

            for (auto& buyLevel : mBuyPrices) {
                // Ignore levels with no more qty
                if (buyLevel.qty == 0) {
                    continue;
                }

                // Determine QTY traded at this level
                auto tradedQty = std::min(trade.qty, buyLevel.qty);
                buyLevel.qty -= tradedQty;
                trade.qty -= tradedQty;

                // Track the total qty and price traded for average fill price
                qtyTraded += tradedQty;
                priceTraded += (tradedQty * buyLevel.price);

                // Check if trade has been executed
                if (trade.qty == 0) {
                    break;
                }
            }

            // Output the trade information
            if (qtyTraded != 0) {
                auto avgFillPrice = priceTraded / qtyTraded;
                outputTrade("BUY", qtyTraded, avgFillPrice, trade.isHedge);
            }
        }

        // Executes a SELL on the book
        void executeSell(Trade& trade) {
            int qtyTraded = 0;
            double priceTraded = 0;

            for (auto& sellLevel : mSellPrices) {
                // Ignore levels with no more qty
                if (sellLevel.qty == 0) {
                    continue;
                }

                // Determine QTY traded at this level
                auto tradedQty = std::min(-trade.qty, sellLevel.qty);
                sellLevel.qty -= tradedQty;
                trade.qty += tradedQty;

                // Track the total qty and price traded for average fill price
                qtyTraded += tradedQty;
                priceTraded += (tradedQty * sellLevel.price);

                // Check if trade has been executed
                if (trade.qty == 0) {
                    break;
                }
            }

            // Output the trade information
            if (qtyTraded != 0) {
                auto avgFillPrice = priceTraded / qtyTraded;
                outputTrade("SELL", qtyTraded, avgFillPrice, trade.isHedge);
            }
        }

        // Helper to Output a Trade
        void outputTrade(const std::string& side, int qty, double avgFillPrice, bool isHedgeTrade) {
            std::cout << std::fixed << std::setprecision(2);
            std::cout << side << ": " << qty << " at $" << avgFillPrice;
            if (isHedgeTrade) {
                std::cout << " - HEDGE";
            }
            std::cout << "\n";
        }

        // Stores the rolling risk each trade takes on
        // - rolling total since a HedgeTrade can only be a whole number
        double mRollingRisk = 0.0;

        // Stores the PriceLevels for each type
        std::vector<PriceLevel> mBuyPrices;
        std::vector<PriceLevel> mSellPrices;
};


int main()
{
    // Guarantee 3 qty/price for each
    std::vector<PriceLevel> buyPrices(3);
    std::vector<PriceLevel> sellPrices(3);

    // Read buy book
    for (int i = 0; i < 3; i++) {
        std::cin >> buyPrices[i].qty >> buyPrices[i].price;
    }

    // Read sell book
    for (int i = 0; i < 3; i++) {
        std::cin >> sellPrices[i].qty >> sellPrices[i].price;
    }

    // Create our RiskHandler giving it the Buy and Sell prices
    RiskHandler riskHandler(std::move(buyPrices), std::move(sellPrices));

    std::string line;

    // Read commands from stdin until EOF.
    while (getline(std::cin, line)) {
        if (line.empty()) {
			continue;
		}

        std::stringstream ss(line);

        int qty;
        double risk;
        ss >> qty >> risk;

        Trade trade{qty, risk, false};

        // process the Trade
        riskHandler.processTrade(std::move(trade));
    }

    std::cout << "\n";

    return 0;
}