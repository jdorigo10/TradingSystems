# Monitoring
Run alongside the pipeline, not inside it, subscribes to events emitted by core components to the Event Bus

## [Trade Logger](./TradeLogger/README.md) - _TODO_
Persists system events (orders, trades, and market data) for audit, replay, and debugging

## [Latency Monitor](./LatencyMonitor/README.md) - _TODO_
Measures latency between key system stages such as: Market data receive → strategy decision, Strategy decision → order submission, Order submission → exchange acknowledgment
