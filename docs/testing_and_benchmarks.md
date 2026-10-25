# Testing and Benchmarks

> **Draft. Not final.** Starting point and reference only. Targets and methods will change.
> Related: [design.md](design.md), [protocol.md](protocol.md).

## 1. Testing

| Test | Purpose |
|---|---|
| Unit tests per rule (GoogleTest) | Full fill, partial fill, multi-level match, cancel, both modify cases, market order, every reject, disconnect |
| Differential test | Slow vector-scan reference matcher against the real book on large random order sets |
| Replay test | Live output hash equals replay output hash |
| Gateway fuzz test | Random bytes must never crash. Run under ASan and UBSan |
| Invariant checks | No overfill, no trade outside either limit price, book never crossed after a command |
| Concurrency tests | ThreadSanitizer plus stress test (Phase B only) |

Rules:
- Tests are deterministic: fixed seeds, no sleeps, no wall clock.
- Pass `seq` and `timestamp` to the engine explicitly.
- The reference matcher lives in `tests/reference/`, stays obviously simple, and is never shipped.
- Add a test for `Quantity` overflow in sums and market sweeps.

First test case (hand-simulated):

| Step | Action | Result |
|---|---|---|
| 1 | Sell 10 @ 100 (id 1) | Rests |
| 2 | Sell 5 @ 99 (id 2) | Rests, best ask |
| 3 | Sell 5 @ 100 (id 3) | Rests behind id 1 |
| 4 | Buy 12 @ 100 (id 4) | Trade with id 2: 5 @ 99. Trade with id 1: 7 @ 100 |

Remaining: id 1 (3) then id 3 (5) at 100. Buy side empty.

## 2. Benchmarks

### Method

- Release build only. Record CPU, cores, RAM, OS and compiler version with every result.
- Two levels: engine only (no network), then end to end over TCP.
- **Open-loop load:** send on a fixed schedule and measure latency from the *scheduled* send time. A closed loop that waits for each reply hides queueing delay (coordinated omission).
- Warm up first, then measure. Report the run length.
- Report p50, p99, p99.9, max and throughput using HdrHistogram. Never report only the average.

### Target

At least 100,000 orders per second sustained, with latency numbers.

### Optimization loop

1. Measure. 2. Profile. 3. Change one thing. 4. Measure again.

No optimization without a benchmark number. Candidates only after a profile points at them: flat-array book, memory pools, core pinning.

### Results table (fill in)

| Date | Build | Level | Orders/sec | p50 | p99 | p99.9 | max | Change |
|---|---|---|---|---|---|---|---|---|
| | | | | | | | | baseline |

## 3. Sanitizers

- ASan and UBSan on all unit, differential and fuzz tests.
- TSan on concurrency tests.
- Presets: `asan`, `tsan`. Never benchmark a sanitizer build.