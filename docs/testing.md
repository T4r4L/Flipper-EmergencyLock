# Testing

## Host tests

Run the test suite with:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

`tests/test_lock_state.c` covers record round-trip and restart selection, partial-write rejection with an older valid slot, checksum corruption, idempotent transitions, generation overflow, and exact-path/confirmation cleanup policy.

These tests are host-side unit tests. They do not exercise MCU flash write atomicity, filesystem behavior, display initialization, watchdog behavior, application isolation, boot sequencing, updater compatibility, or DFU recovery.

## Hardware tests required

No physical Flipper Zero testing has been performed. A device validation plan must cover repeated power cuts during each slot write, flash endurance and retention, startup behavior for missing/invalid records, display and input behavior, watchdog servicing, stock-menu isolation, updater interruption, and recovery through DFU. Test only on a device with a verified recovery path and backed-up data; preserve bootloader and factory regions.

## CI

GitHub Actions builds the host library and test executable and runs CTest. Passing CI is not evidence of hardware verification or a firmware build.
