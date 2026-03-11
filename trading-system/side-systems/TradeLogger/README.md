# Trade Logger
Persists system events (orders, trades, and market data) for audit, replay, and debugging.
- **Input**: Market events, Order events, Trade executions  
    └─ Subscribed to Event Bus
- **Output**: log storage / database entries