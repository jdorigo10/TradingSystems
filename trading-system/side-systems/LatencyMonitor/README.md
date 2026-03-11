# Latency Monitor
Measures latency between key system stages such as: Market data receive → strategy decision, Strategy decision → order submission, Order submission → exchange acknowledgment
- **Input**: Market event receive time, Order submission time, Trade execution time  
    └─ Subscribed to Event Bus
- **Output**: Latency metrics and performance logs for monitoring systems