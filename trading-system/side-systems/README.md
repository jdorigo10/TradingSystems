# Side Systems
Run alongside the pipeline, not inside it, subscribes to events emitted by core components to the Event Bus

```
         Side Systems
------------------------------
Event Bus
    │
    ├────────► Order Tracker
    |
    ├────────► Position & PnL Tracker
    │
    ├────────► Offset Hedger ─────► Order Gateway
    │
    ├────────► Trade Logger
    │
    └────────► Latency Monitor
```

## [Order Tracker](./OrderTracker/README.md)
Maintains lifecycle state of each order

## [Position & PnL Tracker](./PositionPnLTracker/README.md)
Maintains position per symbol, calculates realized and unrealized PnL

## [Offset Hedger](./OffsetHedger/README.md)
Generates hedge trades to offset risk exposure

## [Trade Logger](./TradeLogger/README.md) - _TODO_
Persists system events (orders, trades, and market data) for audit, replay, and debugging

## [Latency Monitor](./LatencyMonitor/README.md) - _TODO_
Measures latency between key system stages such as: Market data receive → strategy decision, Strategy decision → order submission, Order submission → exchange acknowledgment
