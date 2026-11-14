# Architecture

## Current code

`src/lock_state.c` is a portable C11 module without Flipper SDK dependencies. It encodes a 20-byte record:

| Offset | Size | Value |
| --- | ---: | --- |
| 0 | 4 | Magic (`ELK1`) |
| 4 | 4 | Format version (little endian) |
| 8 | 4 | Monotonic generation |
| 12 | 4 | Lock flag (`0` or `1`) |
| 16 | 4 | CRC-32/ISO-HDLC over bytes 0–15 |

The reader validates length, magic, version, flag, and checksum. Given two slots, it selects the valid record with the highest generation. If generations tie, locked wins. If neither validates, the caller receives failure and must not infer an unlocked state. Generation wrap is rejected. Repeating the current transition leaves its generation unchanged.

An adapter should write only the inactive slot, close/sync it, read it back and validate it, and only then treat the new state as committed. The old valid slot is retained. The current repository does not implement such an adapter, so power-loss behavior is tested only at the record-selection boundary.

The test cleanup predicate only accepts the exact `/ext/emergency_lock_test` path and a positive confirmation. It does not perform filesystem deletion. A future adapter must use directory-relative operations, reject symlinks and traversal, and never fall back to formatting or recursive deletion of a broader path.

## Intended device integration

A complete device-wide mode would require changes to the official firmware's boot/application startup and UI routing, plus a persistent internal-flash adapter. This project has not imported or modified upstream firmware. The official build tool supports independent external FAP apps, but an FAP cannot disable the stock menu or enforce a device-wide lock; it is not equivalent to the requested firmware behavior.

Any later integration must keep the stock bootloader, DFU, factory data, and recovery firmware intact. DFU must remain usable without consulting the lock state. The UI must fail closed when state is corrupt, while still retaining the hardware recovery route.

## Data flow (planned)

```text
startup -> read both internal slots -> validate -> choose newest valid record
        -> locked: show fixed lock screen; normal UI withheld
        -> invalid/unavailable: report error and preserve DFU recovery
```

The diagram describes a future design, not current runtime behavior.
