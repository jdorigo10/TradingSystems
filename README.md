# Helpful Trading Systems

Example of how a trading system may flow...

Raw Data -> `MarketFeedHandler` -> `MarketDataStream` -> `MatchingEngine` -> `OrderTracker`|`PositionPnLTracker` -> `OffsetHedger` -> Risk/Limit Monitor

## Market Feed Handler
- recieves raw market messages (trades, quotes, updates)
- parses/normalizes them into a standard internal format
- forwards them to downstream systems (aggregators, risk systems, trading engines)

## Market Data Stream
- aggregates per-symbol stats (VWAP, Last Price, Total Volume)

## Matching Engine
- matches orders based on best price
- First In First Out for same price
- executes trades

## Order Tracker
- tracks state of each order: NEW, PARTIALLY_FILLED, FILLED, CANCELLED
- track order details: ID, symbol, BUY/SELL, qty, filled qty, price
- allows updates for: FILL event, CANCEL event, NEW event

## Position & PnL Tracker
- position: net shares currently held
- avgPrice: average cost basis of the position
- realizedPnL: profit/loss from closed trades
- lastMark: most recent market price (for unrealized PnL)

## Offset Hedger
- computes and hedges trades based on the risk for each
- would then execute a Hedge trade

## Build and Run examples in VS Code via `Tasks` (ctrl + shft + p)
