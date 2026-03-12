# C++ Trading Systems
This repository contains simplified C++ implementations of core components commonly found in electronic trading systems.

The examples demonstrate how market data is processed, trading decisions are generated, and orders are managed and executed.  
The system is divided into three primary components:  
1. Market Data Processing - _Main Pipeline_
2. Order Execution - _Main Pipeline_
3. Event Monitoring / Risk Systems  - _Side Systems_

Some components (such as the Matching Engine) are implemented as standalone simulations to demonstrate how exchanges match orders using price-time priority.

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
Rate Limiter                        |
   │                                ├── Order Execution
   ▼                                │
Order Gateway                       │
   │                                │
   ├────► Event Bus                 │
   |                                │
   ▼                                │
Exchange                          ──┘


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


     Standalone Component
------------------------------
Orders (BUY / SELL / CANCEL)
        │
        ▼
Matching Engine
        │
        ▼
Trade Executions
```

## [Trading System -> Main Pipeline](./trading-system/main-pipeline/README.md)
Processes incoming data and executes trading orders

### [Market Feed Handler](./trading-system/main-pipeline/MarketFeedHandler/README.md)

### [Order Book Builder](./trading-system/main-pipeline/OrderBookBuilder/README.md) - _TODO_

### [Market Data Stream](./trading-system/main-pipeline/MarketDataStream/README.md) - _UPDATE_

### [Strategy Engine](./trading-system/main-pipeline/StrategyEngine/README.md) - _TODO_

### [Risk Manager](./trading-system/main-pipeline/RiskManager/README.md) - _TODO_

### [Rate Limiter](./trading-system/main-pipeline/RateLimiter/README.md) - _TODO_

### [Order Gateway](./trading-system/main-pipeline/OrderGateway/README.md) - _TODO_

## [Trading System -> Side Systems](./trading-system/side-systems/README.md)
Run alongside the pipeline, not inside it, subscribes to events emitted by core components to the Event Bus

### [Order Tracker](./trading-system/side-systems/OrderTracker/README.md) - _UPDATE_

### [Position & PnL Tracker](./trading-system/side-systems/PositionPnLTracker/README.md) - _UPDATE_

### [Offset Hedger](./trading-system/side-systems/OffsetHedger/README.md) - _UPDATE_

### [Trade Logger](./trading-system/side-systems/TradeLogger/README.md) - _TODO_

### [Latency Monitor](./trading-system/side-systems/LatencyMonitor/README.md) - _TODO_

## [Standalone Components](./standalone-component/README.md)
For real trading systems, exchanges (NASDAQ, NYSE, etc) perform order matching. The Matching Engine implementation in this repository is provided as a standalone simulation to demonstrate how price-time priority matching works internally.

### [Matching Engine](./standalone-component/MatchingEngine/README.md) - _UPDATE_
