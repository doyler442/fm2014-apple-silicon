# Football Manager 2014 on Apple silicon

Experimental compatibility fixes and a Steam launcher for the **original 32-bit Mac edition of Football Manager 2014**, built on [AArchX](https://github.com/mont127/AArchX).

This project began with an owned Steam copy that modern macOS would not launch. A locally patched AArchX build now starts that copy, loads an old career, accepts input, and resizes correctly on the tested Mac. It runs on macOS without installing Windows or buying compatibility software.

**This is an experimental source release, not a generally verified installer.** Full match gameplay, saving and reloading, and long sessions have not yet been verified. Back up saves before trying it.

## What has been tested

| Item | Result |
| --- | --- |
| Hardware | Apple M6 Mac mini |
| macOS | 27.0.1 |
| Game | Steam Mac edition, 14.3.1 |
| Build SDK | macOS SDK 26.2, Apple Command Line Tools |
| Startup and main menu | Confirmed by the user |
| Mouse, keyboard and text entry | Confirmed by the user |
| Existing career | An old transferred save loads |
| Dropdowns | Stay open after a single click |
| Resizing and pointer alignment | Confirmed by the user |
| Full matches, save/reload and long sessions | Not verified |
| Fullscreen and other Macs | Not verified |

These fixes are not tied to a machine identifier. Some address general compatibility-layer gaps, but results on other hardware, macOS releases or games need independent testing.

## Requirements

- An Apple silicon Mac and Apple's Command Line Tools (`xcode-select --install`).
- Git, a network connection and an owned, installed Steam Mac copy of FM2014.
- The Steam account that owns the game, signed in normally.

The game is not included. No game executable, save file, Steam credentials or Apple system library is distributed here.

## Build

After downloading or cloning this repository, run in Terminal:

```sh
bash build.sh
```

The script fetches the pinned AArchX revision, applies the patch, builds the translator, generates API databases using your installed SDK, and builds the launcher. Upstream tools also fetch pinned Steam interface headers from Valve's Proton repository and checksum-verified C++ runtime packages from MacPorts. Those dependencies stay in the ignored `build/` directory and are not included in this source release. See their respective licences before redistributing them.

The completed files are in `build/`. The script prints a Steam Launch Options line containing the absolute launcher path. Copy that line into **Football Manager 2014 → Properties → General → Launch Options**, then use Steam's **Play** button.

Keep the repository and its `build/` folder in place. If you move them, update the path in Steam. To return to Steam's normal launch behaviour, clear Launch Options. The script does not edit Steam settings, install system components or alter the game files.

The build helper has local syntax and patch-application checks; a complete clean build through the helper and a second-machine installation still need verification. SDK differences can expose additional build issues.

## What changed

The patch includes the translator changes and regression tests developed while investigating FM2014:

- SDK availability guards for older or missing declarations.
- Correct disposal of a host-owned network-interface list.
- Main-thread graphics dispatch, graphics-lock handoff and servicing main-queue work during ordinary and timed waits.
- CoreFoundation string-formatting conversion for i386 variadic calls.
- An opt-in Cocoa event loop for mouse and keyboard delivery.
- Host protocol signature lookup for text-input callbacks with architecture-dependent ranges.
- Consistent clock units for `DurationToAbsolute` and the virtual `UpTime` clock, correcting dropdown press timing.
- A `sem_open` bridge for optional mode/count arguments, restoring queued resize events and pointer alignment.

Some behaviours are deliberately opt-in through environment settings enabled by the launcher. The patch is pinned to upstream commit `9838c317d2c346b4694ca94e7f772106be8a56d0`; do not assume it applies unchanged to the latest AArchX.

## Regression checks

From this repository after building:

```sh
cd build/AArchX
make -j6 tests/unit/bin/test_m32
OCERZ_NO_ARM_EXEC=1 tests/unit/bin/test_m32 namedsem
OCERZ_NO_ARM_EXEC=1 tests/unit/bin/test_m32 clockunits
OCERZ_NO_ARM_EXEC=1 tests/unit/bin/test_m32 timedpump
OCERZ_NO_ARM_EXEC=1 tests/unit/bin/test_m32 protocolrange
OCERZ_NO_ARM_EXEC=1 tests/unit/bin/test_m32 cfappend
OCERZ_NO_ARM_EXEC=1 tests/unit/bin/test_m32 gllock
OCERZ_M32_MAIN_WAIT_PUMP=1 OCERZ_M32_GL_LOCK_HANDOFF=1 bash tests/run_m32_tests.sh threads cfformat objc_basic objc_struct
```

During development, named semaphores passed 27 checks, clock units 45, timed waits 29, protocol ranges 4, string append 7, and graphics-lock handoff 10. Existing threading, formatting and Objective-C execution checks also passed. These checks support the fixes; they do not establish complete game compatibility.

## Troubleshooting and feedback

The launcher writes `build/fm2014.log`, replacing it on the next launch. If the game fails, preserve that log and report your hardware, macOS version, SDK version, game version and the last operation that worked. Review logs for personal information before posting. Do not upload saves, account details, proprietary game files or SDK headers.

Startup may still report unsupported legacy Chromium features. Browser/social features and fullscreen are unverified. Report an actual symptom alongside warnings, because a warning alone does not identify the cause.

## Credit and licence

[AArchX](https://github.com/mont127/AArchX), maintained by mont127, supplies the translation engine and system bridges. This repository contains additional compatibility changes and a small launcher developed by Cathal Doyle with Codex (Digital Lava), 23–24 October 2026. It does not claim authorship of the upstream engine.

The patched AArchX code is distributed under **LGPL-2.1-or-later**, retaining upstream notices; the launcher and build helper are supplied under the same licence. See [LICENSE](LICENSE), [UPSTREAM](UPSTREAM) and [CHANGES.md](CHANGES.md). Dependencies retain their own licences. This project is independent of Sports Interactive, SEGA, Valve and Apple.

[Read the account of the experiment](https://cathaldoyle.com/My_Mac_said_no._I_just_wanted_to_play_Football_Manager).
