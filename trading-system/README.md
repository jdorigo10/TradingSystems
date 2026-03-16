# Trading System
Processes incoming data and executes trading orders

## [Market Feed Handler](./MarketFeedHandler/README.md)
Parses raw feed messages, converts them into internal normalized structures

## [Order Book Builder](./OrderBookManager/README.md)
Maintains the current bid/ask order book for each symbol, reconstructs exchange book from incremental updates

## [Market Data Stream](./MarketDataManager/README.md)
Aggregates market statistics such as: VWAP, Last Traded Price, Volume, Rolling Averages

## [Strategy Engine](./StrategyEngine/README.md)
Generates trading signals based on market conditions

## [Risk Manager](./RiskManager/README.md) - _TODO_
Validates risk limits before allowing order submission, checks: Max Position, Max Order Size, Daily Loss Limits

## [Rate Limiter](./RateLimiter/README.md) - _TODO_
Prevents sending too many orders to the exchange, ensures compliance with exchange rate limits

## [Order Gateway](./OrderGateway/README.md) - _TODO_
Handles communication with exchange APIs, sends orders and receives execution reports

## [Order Tracker](./OrderTracker/README.md) - _UPDATE_
Maintains lifecycle state of each order

## [Position & PnL Tracker](./PositionTracker/README.md) - _UPDATE_
Maintains position per symbol, calculates realized and unrealized PnL

## [Offset Hedger](./OffsetHedger/README.md) - _UPDATE_
Generates hedge trades to offset risk exposure

## [Trade Logger](./TradeLogger/README.md) - _TODO_
Persists system events (orders, trades, and market data) for audit, replay, and debugging

## [Latency Monitor](./LatencyMonitor/README.md) - _TODO_
Measures latency between key system stages such as: Market data receive → strategy decision, Strategy decision → order submission, Order submission → exchange acknowledgment