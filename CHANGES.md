# Local changes, 23–24 October 2026

This source release applies `patches/aarchx-fm2014.patch` to AArchX commit `9838c317d2c346b4694ca94e7f772106be8a56d0`. The original project's author and licence notices remain in its source. Local modifications were developed for Cathal Doyle's FM2014 compatibility experiment with Digital Lava (Codex, GPT-6.1 Sol, medium reasoning).

| Area | Reason |
| --- | --- |
| `src/dyldapi.c`, SDK header input | Build with the installed macOS 26.2 SDK |
| `src/m32_cross.c` | Pass host-owned `ifaddrs` allocations to their destructor; optional diagnostic tracing |
| `src/m32_objc.c` | Graphics main-thread routing, lock handoff and protocol-only callback signatures; optional window diagnostics |
| `src/m32_gl.c`, `src/m32_leaf.c`, `include/ocerz/m32.h` | Track and transfer recursive graphics locks around synchronous main-thread work |
| `src/m32_thread.c` | Opt-in main-loop service during plain and timed condition waits |
| `src/m32_cf.c` | Convert i386 variadic CoreFoundation append-format arguments |
| `src/m32_libsystem.c` | Opt-in Cocoa event loop, consistent virtual clock conversion and named-semaphore creation |
| `src/m32_callback.c` | Optional input callback diagnostics |
| `tests/unit/test_m32.c` | Regression checks for the corrected behaviours |

The build helper generates platform API descriptions locally and obtains separately licensed dependencies through upstream tools. This repository intentionally distributes source and a patch only. No compiled translator, runtime libraries, Apple or Valve headers, game files, saves, logs or credentials are included.
