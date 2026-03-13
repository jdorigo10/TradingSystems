#include <algorithm>

#include <mp/StrategyEngine.hpp>

namespace mp {

auto StrategyEngine::onMarketUpdate(const std::string &symbol) -> std::optional<common::OrderRequest> {
  // Get the OrderBook & MarketStats for the Symbol
  const auto &book = mBookManager.getBook(symbol);
  const auto &stats = mDataManager.getStats(symbol);
  if (!book.has_value() || !stats.has_value()) {
    return std::nullopt; // NO REQUEST: either no book or stats for symbol
  }

  // Get the BestBid & BestAsk from the Book
  const auto &bestBid = book->getBestBid();
  const auto &bestAsk = book->getBestAsk();
  if (!bestBid.has_value() || !bestAsk.has_value()) {
    return std::nullopt; // NO REQUEST: either no best bid or ask on book
  }

  // The spread represents the cost of crossing the market.
  // - A tight spread means less opportunity for edge.
  double spread = bestAsk->price - bestBid->price;

  // Positive signal  -> price above fair value
  // Negative signal  -> price below fair value
  double signal = stats->lastTradePrice - stats->vwap;

  /*** DETERMINE IF A TRADE SHOULD BE MADE ***/

  // If the spread is too tight OR the signal is too small, trading is not worthwhile
  if ((spread < 0.01) || (signal >= -0.02 && signal <= 0.02)) {
    return std::nullopt;
  }

  // Create an OrderRequest
  common::OrderRequest orderRequest;
  orderRequest.symbol = symbol;
  orderRequest.qty = determineOrderSize(*bestBid, *bestAsk);

  if (std::abs(signal) > 0.05) {
    // strong signal, act immediately
    orderRequest.type = common::OrderRequest::Type::MARKET;
  } else {
    // try to capture spread
    orderRequest.type = common::OrderRequest::Type::LIMIT;
  }

  if (signal < -0.02) {
    // price is below VWAP → undervalued → BUY
    orderRequest.side = common::OrderRequest::Side::BUY;

    // LIMIT: place at best bid to capture spread
    // MARKET: price field can be ignored or set to best ask
    orderRequest.price = (orderRequest.type == common::OrderRequest::Type::LIMIT) ? bestBid->price : bestAsk->price;
  } else if (signal > 0.02) {
    // price above VWAP → overvalued → SELL
    orderRequest.side = common::OrderRequest::Side::SELL;

    // LIMIT: place at best ask to capture spread
    // MARKET: price field can be ignored or set to best bid
    orderRequest.price = (orderRequest.type == common::OrderRequest::Type::LIMIT) ? bestAsk->price : bestBid->price;
  }

  orderRequest.print();

  return orderRequest;
}

auto StrategyEngine::determineOrderSize(const common::Order &bestBid, const common::Order &bestAsk) -> int {
  int bidSize = bestBid.qty;
  int askSize = bestAsk.qty;

  // Use the smaller side of liquidity
  int topLiquidity = std::min(bidSize, askSize);

  // Trade a fraction of visible liquidity
  int qty = topLiquidity / 5;

  // Prevent extremely small or large orders
  return std::clamp(qty, 10, 100);
}

} // namespace mp
