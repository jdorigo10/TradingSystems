# Market Feed Handler
Parses raw feed messages, converts them into internal normalized structures
- **Input**: Raw exchange messages (trades, book updates)
- **Output**: TradeEvent and OrderUpdate objects forwarded to internal systems  
    └─ Published to Event Bus