---
description: Review the current change for correctness, determinism, and undefined behavior
agent: agent
tools: ['search', 'read']
---

Review the current changes. Do not edit files.

## Check, in order
1. **Correctness**: matching rules, priority, partial fills, modify and cancel behavior.
2. **Determinism**: any clock, randomness, unordered iteration, or thread dependence inside `matching_core`.
3. **Memory and UB**: dangling iterators or references, integer overflow (`Quantity` is 32-bit), uninitialized fields.
4. **Concurrency**: data races, shared state across threads.
5. **Layering**: includes that break the dependency direction.
6. **Tests**: missing cases for the changed behavior.
7. **Clarity**: naming, unnecessary complexity.

## Output
Findings grouped as Must fix / Should fix / Nit, each with file and line.
If nothing is wrong, say so plainly. Do not invent issues.