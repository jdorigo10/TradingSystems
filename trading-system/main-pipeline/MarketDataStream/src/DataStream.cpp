#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>

/* Represents running statistics for a symbol
 *
 * Why struct?
 * - Instead of recomputing stats from historical trades (which would be slow),
 *   we maintain running totals that allow O(1) updates per trade.
*/
struct MarketStats {
    long totalVolume = 0;      // Sum of all traded qtys
    double priceVolume = 0.0;  // Sum of (price * qty) used for VWAP
    double lastPrice = 0.0;    // Most recent trade price
};

/* MarketDataStream
 * - aggregates per-symbol stats
 */
class DataStreamer {
    public:
        // Process a single trade event
        void processTrade(const std::string& symbol, double price, int qty) {
            // Ignore trades that dont have any qty
            if (qty == 0) {
                return;
            }

            // Access stats for symbol, if it doesnt exist - create default entry
            MarketStats& stats = mMarket[symbol];

            // Update the total traded volume
            stats.totalVolume += qty;

            // Update the price-volume accumulator
            stats.priceVolume += (price * qty);

            // Update last traded price
            stats.lastPrice = price;
        }

        // Prints the stats for a symbol
        void printStats(const std::string& symbol) {
            // Get stats for the symbol, if it doesnt exist - create default entry
            const MarketStats& stats = mMarket[symbol];
            
            std::cout << "+----- " << symbol << " -----+\n"
                      << " Last Price: " << stats.lastPrice << "\n"
                      << "  Total Vol: " << stats.totalVolume << "\n"
                      << "       VWAP: " << getVWAP(symbol) << "\n"
                      << std::endl;
        }

    private:
        // Computes Volume Weighted Average Price for a symbol
        double getVWAP(const std::string& symbol) {
            // Get stats for the symbol, if it doesnt exist - create default entry
            const MarketStats& stats = mMarket[symbol];

            if (stats.totalVolume == 0) {
                return 0.0;
            }

            // VWAP = total price-volume / total volume
            return stats.priceVolume / stats.totalVolume;
        }

        /* Maps: SYMBOL -> stats for each
         *
         * Why unordered_map:
         * - O(1) average lookup
         * - symbols like "AAPL", "MSFT" are natural keys
         * - trading systems process thousands of symbols quickly
        */
        std::unordered_map<std::string, MarketStats> mMarket;
};


int main()
{
    DataStreamer streamer;

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
            std::string symbol;
            double price;
            int qty;
            ss >> symbol >> price >> qty;

            streamer.processTrade(symbol, price, qty);
        }
        else if (cmd == "PRINT")
        {
            std::string symbol;
            ss >> symbol;

            streamer.printStats(symbol);
        }
        else
        {
            std::cout << "Unknown command\n";
        }
    }

    return 0;
}