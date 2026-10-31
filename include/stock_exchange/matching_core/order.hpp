#pragma once

#include <stock_exchange/matching_core/domain_types.hpp>

namespace stock_exchange::matching_core {

enum class Side {
    buy,
    sell,
};

enum class OrderType {
    limit,
    market,
};

struct Order {
    SymbolId symbol{};
    SessionId session{};
    ClientOrderId client_order_id{};
    Side side{};
    OrderType type{OrderType::limit};
    PriceTicks price{};
    Quantity quantity{};
    SeqNo sequence{};
};

}  // namespace stock_exchange::matching_core
