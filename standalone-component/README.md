# Standalone Component
For real trading systems, exchanges (NASDAQ, NYSE, etc) perform order matching. The Matching Engine implementation in this repository is provided as a standalone simulation to demonstrate how price-time priority matching works internally.

```
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

## [Matching Engine](./MatchingEngine/README.md)
Matches orders based on price-time priority, executes trades when bid >= ask
