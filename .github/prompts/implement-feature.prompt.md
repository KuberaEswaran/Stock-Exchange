---
description: Implement one small feature or step of the exchange, with tests
agent: agent
---

Implement: ${input:feature:Describe the feature or step}

## Steps
1. Read `.github/copilot-instructions.md` and check the feature is in scope.
2. State the plan in at most 5 bullets before writing code.
3. Write the smallest change that works. Do not refactor unrelated code.
4. Add or update tests for every new rule.
5. Build and run the tests. Report the result.

## Constraints
- Respect the dependency direction and the `matching_core` purity rules.
- No optimization without a benchmark.
- If a design decision is unclear, ask one question instead of guessing.

## Final response
List the files changed, the tests added, and any trade-off made.