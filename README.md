# Stock-Exchange

## Source Layout

Public C++ headers are kept separate from implementation files:

```text
include/
└── stock_exchange/
    └── matching_core/
        ├── domain_types.hpp
        ├── order.hpp
        ├── order_book.hpp
        └── matching_engine.hpp

src/
└── matching_core/
    ├── order_book.cpp
    └── matching_engine.cpp
```

Include public headers with the stable project path:

```cpp
#include <stock_exchange/matching_core/order.hpp>
```

The `include/` tree defines the public matching-core API. The `src/` tree
contains implementation details.