# Recovery and DFU

## Current prototype

The project has not been flashed to a device and does not change firmware, bootloader, factory data, OTP, radio stack, or microSD contents. There is no project-specific recovery operation.

## If a future firmware build is installed

Use the official [Flipper Zero firmware recovery/update guide](https://docs.flipper.net/basics/firmware-update/official) and the supported official desktop/mobile updater. Follow the current vendor instructions for the exact hardware revision. The bootloader/DFU path must not be modified by this project.

DFU recovery is a firmware recovery mechanism, not a data-wipe guarantee. Do not assume that it erases microSD content, restores all internal settings, removes previously written NAND pages, or returns every partition to factory state. Back up data before experimentation.

The behavior of an interrupted installation depends on the updater and device state; use the official recovery instructions if the device does not boot. This repository has not tested interruption or recovery on physical hardware. No update package is produced because the project has no integrated firmware target and package compatibility has not been verified.
