#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

// Represents a normalized market data tick
struct Tick {
    std::string symbol;
    std::string type;    // TRADE or QUOTE
    
    double price = 0.0;  // trade price or quote mid price
    int qty = 0;         // trade qty   or 0 for quotes

    double bid = 0.0;  // for quotes
    double ask = 0.0;  // for quotes

    void print() const {
        // Allow 4 decimals
        std::cout << std::fixed << std::setprecision(4);
        std::cout << "Tick: " << type << " " << symbol;
        if(type == "TRADE") {
            std::cout << " price=" << price << " qty=" << qty;
        } else {
            std::cout << " bid=" << bid << " ask=" << ask;
        }
        std::cout << std::endl;
    }
};

/* Market Data Feed Handler
 * - recieves raw market messages (trades, quotes, updates)
 * - parses/normalizes them into a standard internal format
 * - forwards them to downstream systems (aggregators, risk systems, trading engines)
*/
class FeedHandler {
    public:
        // Handle a TRADE message
        void processTrade(const std::string& symbol, double price, int qty) {
            Tick tick;
            tick.symbol = symbol;
            tick.type = "TRADE";
            tick.price = price;
            tick.qty = qty;

            // In a real system, forward this tick to downstream consumer
            mTicks.push_back(tick);

            // For this we will just print it
            tick.print();
        }

        // Handles a QUOTE message
        void processQuote(const std::string& symbol, double bid, double ask) {
            Tick tick;
            tick.symbol = symbol;
            tick.type = "QUOTE";
            tick.bid = bid;
            tick.ask = ask;

            // In a real system, forward this tick to downstream consumer
            mTicks.push_back(tick);

            // For this we will just print it
            tick.print();
        }

    private:
        // Store ticks temporarily (for print, debugging, later forwarding)
        // - in a real system, might want a short-term cache of ticks to 
        //   replay to new downstream consumers or to batch-process
        // - usually use a queue or ring buffer
        std::vector<Tick> mTicks;
};


int main()
{
    FeedHandler handler;

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

            handler.processTrade(symbol, price, qty);
        }
        else if (cmd == "QUOTE")
        {
            std::string symbol;
            double bid, ask;
            ss >> symbol >> bid >> ask;

            handler.processQuote(symbol, bid, ask);
        }
        else
        {
            std::cout << "Unknown command\n";
        }
    }

    return 0;
}