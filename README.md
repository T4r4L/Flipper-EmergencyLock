# Flipper EmergencyLock prototype

Flipper EmergencyLock is an early, GPL-3.0 project exploring a recoverable emergency-lock design for Flipper Zero. **The current code is a portable lock-record and test-policy library; it is not Flipper firmware and does not lock a device.** It does not replace the bootloader, disable the stock menu, encrypt user files, or produce an update package.

## Goals

The intended design is to reduce normal device access during an explicitly activated emergency mode while preserving recovery through the unmodified hardware DFU path. The implementation must fail closed on invalid lock records, avoid touching user data, and keep the bootloader and factory data intact.

## Implemented now

- Versioned, fixed-length lock records with CRC-32 integrity checks.
- Two-slot state selection that tolerates one missing, partial, or corrupt record.
- Monotonic generations, idempotent state transitions, and overflow rejection.
- An exact-path and explicit-confirmation guard for test-data cleanup.
- Host-side C tests and a CMake build.

## Planned, not implemented

- A Flipper Zero application or firmware integration that displays the requested lock message.
- Hardware-backed persistent storage adapter and power-loss testing on device.
- Confidential-file encryption using a vetted AEAD library and documented key provisioning.
- Any firmware update artifact or installation flow.

## Current limitations

The repository started with only a short README and the GPL-3.0 license. No upstream firmware source, application, persistent-storage adapter, or device test harness was present. The state library is not a security boundary: stock Flipper firmware can still be used, and its test-path guard must be combined with carefully constrained filesystem operations by any future adapter. CRC detects accidental corruption; it does not authenticate records against an attacker. No actual record is persisted by this prototype.

Ordinary FAT32 formatting does not guarantee unrecoverable deletion from NAND flash. DFU recovery restores firmware but should not be assumed to erase microSD data or all internal settings. No cryptographic erasure is implemented.

## Build and test

Requirements: CMake 3.16+ and a C11 compiler.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build --output-on-failure
```

The repeatable host build is documented in [docs/build.md](docs/build.md). GitHub Actions runs the same host build and tests. This does not compile firmware or verify hardware behavior.

## Recovery

This prototype does not install to a device, so no recovery action is required. For future firmware work, use the official [Flipper Zero recovery instructions](https://docs.flipper.net/basics/firmware-update/official). The hardware bootloader and DFU path are outside this repository and have not been modified. See [docs/recovery-dfu.md](docs/recovery-dfu.md).

## Security and licensing

Do not use this prototype to protect real files or to claim that a Flipper Zero is locked. Design limits and threat assumptions are in [docs/security-model.md](docs/security-model.md). License and upstream provenance are in [docs/licensing.md](docs/licensing.md).

This project is distributed under GNU GPL version 3; see [LICENSE](LICENSE).
