# Protocol, Gateway and Journal

> **Draft. Not final.** Starting point and reference only. Field sizes, layouts and policies will change.
> Related: [design.md](design.md).

## 1. Wire protocol

- Little-endian, fixed-size packed structs, binary. JSON, if ever needed, stays in a bridge outside the core.
- Frame: `[u16 length][u8 type][u8 version][u64 client_msg_seq][payload]`.
- Symbol on the wire is `char[8]`. The gateway looks it up once and passes a `SymbolId` onward.
- Every response (`Accepted`, `Rejected`, `Cancelled`, `Modified`) echoes the request's `client_msg_seq` as `ref_msg_seq`.
  Trade reports are events, not responses, so they carry no `ref_msg_seq`.
- `client_msg_seq` is **echoed only**, never validated. FIX-style sequence enforcement is future work.

### Framing

TCP is a byte stream. One `recv` may return half a message or several messages. The gateway keeps a per-connection read buffer, extracts complete frames by the length prefix, and keeps leftover bytes for the next read.

### Malformed input

| Case | Action |
|---|---|
| Header decodable, payload bad (short payload, enum out of range) | `Rejected{Malformed, ref_msg_seq}`. Connection stays open |
| Framing broken (length 0 or oversized) | Cannot resync. Send `Rejected{ref=0}` and close |

A reject for a malformed frame has no `SeqNo`, because it never enters the sequenced pipeline. The trader correlates it by `ref_msg_seq`. Malformed frames are not journaled.

## 2. Gateway and sessions

- Non-blocking sockets with `epoll`.
- Per connection: read buffer, bounded write buffer.
- Accept assigns a `SessionId` from a counter. One connection is one trader (login is out of scope).
- Close produces a `SessionClosed` command (cancel-on-disconnect).
- **Slow client policy:** if the write buffer passes its cap, disconnect that client. The engine never blocks on a slow reader.
- Output events carry a target `SessionId` and are routed to that connection.
- Gateway does no business validation. See the split in [design.md](design.md).

## 3. Sequencer

- One place assigns `SeqNo` and `Timestamp`, when the message is received.
- **`SeqNo` is the ordering key.** Timestamps can tie or step backwards, so they are data only.
- "Earlier order" means lower `SeqNo`, not the trader's own clock.

## 4. Journal

Record layout:

| Field | Size |
|---|---|
| length | u16 |
| seq | u64 |
| timestamp | u64 |
| session | u32 |
| client_msg_seq | u64 |
| msg type | u8 |
| payload | fixed per type |
| crc32 | u32 |

The file header holds a format version and the symbol table, so replay uses the same symbols as the live run.

### Write path

1. Sequencer stamps the command.
2. Journal appends it.
3. Engine processes it. The engine never sees a command that is not journaled.

### Durability

- Single append-only file, buffered writes, **no fsync per order**.
- Optional group commit: fsync every N milliseconds.
- Process crash: data is safe, since `write()` has reached the OS buffer. Power loss or OS crash: the last few records may be lost.
- Per-order fsync cannot sustain 100k orders/sec. Real exchanges get durability from replication, which is out of scope. Document this trade-off in the README.

### Replay

- Read records in order and feed the engine with the stored `seq` and `timestamp`. The engine never reads a clock.
- A half-written tail record fails the length or CRC check and is truncated. A CRC failure in the middle of the file is an ERROR.
- After recovery, append a `SessionClosed` for every session that was open, since all connections died with the process.
- Determinism check: hash of live trades equals hash of replayed trades.

## 5. Open questions

- Exact `Order` and record field order and padding.
- Group-commit interval, if used.
- Maximum frame length and write-buffer cap values.
- Whether partitions share one journal file or each have their own (current plan: each their own).