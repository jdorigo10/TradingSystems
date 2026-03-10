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

## Main Pipeline
Processes incoming data and executes trading orders

### Market Feed Handler
Parses raw feed messages, converts them into internal normalized structures
- **Input**: Raw exchange messages (trade, quote, book update)
- **Output**: MarketEvent objects forwarded to internal systems  
    └─ Published to Event Bus

### Order Book Builder - _TODO_
Maintains the current bid/ask order book for each symbol, reconstructs exchange book from incremental updates
- **Input**: Market data updates
- **Output**: Updated order book snapshots

### Market Data Stream
Aggregates market statistics such as: VWAP, Last Traded Price, Volume, Rolling Averages
- **Input**: Market events, Order book updates
- **Output**: Market indicators used by strategies

### Strategy Engine - _TODO_
Generates trading signals based on market conditions
- **Input**: Market statistics, Order book state
- **Output**: Order requests (BUY / SELL)

### Risk Manager - _TODO_
Validates risk limits before allowing order submission, checks: Max Position, Max Order Size, Daily Loss Limits
- **Input**: Proposed orders
- **Output**: Approved or rejected orders

### Rate Limiter / Throttler - _TODO_
Prevents sending too many orders to the exchange, ensures compliance with exchange rate limits
- **Input**: Approved orders
- **Output**: Throttled order stream

### Order Gateway - _TODO_
Handles communication with exchange APIs, sends orders and receives execution reports
- **Input**: Orders approved by system
- **Output**: Exchange confirmations and fills  
    └─ Published to Event Bus

## Side Systems
Run alongside the pipeline, not inside it, subscribes to events emitted by core components to the Event Bus

### Order Tracker
Maintains lifecycle state of each order
- **Input**: Order events (NEW, FILL, CANCEL)  
    └─ Subscribed to Event Bus
- **Output**: Updated order states

### Position & PnL Tracker
Maintains position per symbol, calculates realized and unrealized PnL
- **Input**: Trade executions  
    └─ Subscribed to Event Bus
- **Output**: Updated position and PnL metrics

### Offset Hedger
Generates hedge trades to offset risk exposure
- **Input**: Position updates and Trade executions  
    └─ Subscribed to Event Bus
- **Output**: Hedge order requests

### Trade Logger - _TODO_
Persists system events (orders, trades, and market data) for audit, replay, and debugging.
- **Input**: Market events, Order events, Trade executions  
    └─ Subscribed to Event Bus
- **Output**: log storage / database entries

### Latency Monitor - _TODO_
Measures latency between key system stages such as: Market data receive → strategy decision, Strategy decision → order submission, Order submission → exchange acknowledgment
- **Input**: Market event receive time, Order submission time, Trade execution time  
    └─ Subscribed to Event Bus
- **Output**: Latency metrics and performance logs for monitoring systems

## Standalone Components
For real trading systems, exchanges (NASDAQ, NYSE, etc) perform order matching. The Matching Engine implementation in this repository is provided as a standalone simulation to demonstrate how price-time priority matching works internally.

### Matching Engine
Matches orders based on price-time priority, executes trades when bid >= ask
- **Input**: Orders (BUY, SELL, CANCEL)
- **Output**: Trade executions
