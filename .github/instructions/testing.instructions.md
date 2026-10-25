---
applyTo: "tests/**,**/*_test.cpp,**/*_bench.cpp"
---

# Testing rules

- Framework: GoogleTest. One behavior per test. Name tests `Subject_Condition_Expected`.
- Every rule needs a test: full fill, partial fill, multi-level match, cancel,
  both modify cases (keep position / lose position), market order, every reject, disconnect.
- Tests must be deterministic: no sleeps, no wall clock, fixed random seeds.
- Pass `seq` and `timestamp` explicitly to the engine in tests.
- Keep a slow reference matcher in `tests/reference/` and compare it against the real book
  on random orders (differential test).
- Replay test: live output hash must equal replay output hash.
- Invariants to assert: no overfill, no trade outside either limit price, book never crossed.
- Network code gets fuzz tests with random bytes and must not crash under ASan/UBSan.
- Benchmarks: Release build only, open-loop load, avoid coordinated omission.