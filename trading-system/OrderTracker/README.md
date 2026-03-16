# Order Tracker
Maintains lifecycle state of each order
- **Input**: Order events (NEW, FILL, CANCEL)  
    └─ Subscribed to Event Bus
- **Output**: Updated order states
