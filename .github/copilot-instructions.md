# Mini Stock Exchange

C++23 stock exchange backend. Learning and portfolio project, not production.
Flow: client -> TCP gateway -> sequencer -> journal -> matching engine -> responses.

## Build and test
- Platform: Linux (WSL2). Build: `cmake -S . -B build && cmake --build build`
- Tests: `ctest --test-dir build --output-on-failure` (GoogleTest)
- Benchmarks only in Release builds.

## Architecture rules
- `src/matching_core/` is pure logic: no I/O, no threads, no logging, no clock calls.
- Engine behaves as a pure function: (state, command, seq, timestamp) -> events.
- Dependency direction: exchange_server -> wire_protocol -> matching_core.
  `matching_core` never includes anything from other folders.
- Gateway validates syntax only (framing, length, type, enum ranges).
  Engine validates semantics (symbol, quantity, price, duplicate id, open order).
- Journal stores well-formed commands only. Malformed frames are never journaled.

## Domain rules
- Prices are integer ticks (`int64_t`). Never use floating point for price.
- Priority is price first, then sequence number. Timestamps are data, never ordering keys.
- Trades execute at the resting order's price.
- Orders are keyed by (SessionId, ClientOrderId). Ownership is structural.

## Code style
- Use `enum class`, `using` aliases for domain types, and `std::span`/`std::string_view` over raw pointers.
- No raw `new`/`delete`. No exceptions in the matching hot path.
- Do not optimize without a benchmark number.

## Keep out of scope
Auth, risk checks, settlement, replication, databases.

## Change-friendly workflow
- Treat this file, its content, the folder structure, plans, architecture, and implementation as changeable.
- The current instructions and project content are a starting draft, not final or permanent.
- The user may replace, rewrite, remove, or extend any project guidance at any time.
- Do not preserve existing guidance when the user explicitly changes it.
- Before each task, inspect the current repository and follow the latest code and documentation.
- When requirements change, update affected instructions, tests, and documentation together.
