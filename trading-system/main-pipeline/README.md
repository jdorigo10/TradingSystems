# Main Pipeline
Processes incoming data and executes trading orders

```
        Main Pipeline
------------------------------
Exchange                          ──┐
   │                                │
   ▼                                │
Market Feed Handler                 │
   │                                │
   ├────► Event Bus                 │
   │                                │
   ▼                                ├── Market Data Processing
Order Book Builder                  │
   │                                │
   ▼                                │
Market Data Stream                  │
   │                                │
   ▼                                │
Strategy Engine                   ──┤
   │                                │
   ▼                                │
Risk Manager                        │
   │                                │
   ▼                                │
Rate Limiter                        │
   │                                ├── Order Execution
   ▼                                │
Order Gateway                       │
   │                                │
   ├────► Event Bus                 │
   │                                │
   ▼                                │
Exchange                          ──┘
```

## [Market Feed Handler](./MarketFeedHandler/README.md)
Parses raw feed messages, converts them into internal normalized structures

## [Order Book Builder](./OrderBookBuilder/README.md)
Maintains the current bid/ask order book for each symbol, reconstructs exchange book from incremental updates

## [Market Data Stream](./MarketDataStream/README.md)
Aggregates market statistics such as: VWAP, Last Traded Price, Volume, Rolling Averages

## [Strategy Engine](./StrategyEngine/README.md)
Generates trading signals based on market conditions

## [Risk Manager](./RiskManager/README.md) - _TODO_
Validates risk limits before allowing order submission, checks: Max Position, Max Order Size, Daily Loss Limits

## [Rate Limiter](./RateLimiter/README.md) - _TODO_
Prevents sending too many orders to the exchange, ensures compliance with exchange rate limits

## [Order Gateway](./OrderGateway/README.md) - _TODO_
Handles communication with exchange APIs, sends orders and receives execution reports
