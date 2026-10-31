#pragma once

#include <cstdint>

namespace stock_exchange::matching_core {

using SymbolId = std::uint32_t;
using SessionId = std::uint32_t;
using ClientOrderId = std::uint64_t;
using ClientMessageSeq = std::uint64_t;
using PriceTicks = std::int64_t;
using Quantity = std::uint64_t;
using SeqNo = std::uint64_t;
using Timestamp = std::uint64_t;

}  // namespace stock_exchange::matching_core
