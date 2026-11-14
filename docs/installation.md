# Installation

There is no device-installable artifact in this prototype. The C library and test executable run on a development computer only. Do not copy either binary to a Flipper Zero or treat it as firmware.

To use the host prototype, follow [docs/build.md](build.md). The state format is a design/test aid; it is not currently connected to device storage, menu startup, or a user interface.

A future FAP could be installed using the official FAP workflow, but it would run as an ordinary application and could not disable the stock menu or protect files from other applications. Full firmware integration would need a separately reviewed, version-pinned firmware build and a compatible official update package. No such package has been verified or generated.
