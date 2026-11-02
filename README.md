# Makou Reactor

![Makou Reactor](src/qt/images/logo-shinra.png)

Final Fantasy VII field archive editor. Fork of [myst6re/makoureactor](https://github.com/myst6re/makoureactor)
([releases](https://github.com/myst6re/makoureactor/releases), [forum](http://forums.qhimm.com/index.php?topic=9658.0)).

## Changes in this fork

- **UI font:** Atkinson Hyperlegible, one point larger than the system default.
- **DuckStation attach/reload:** *Attach to DuckStation...* (Settings menu) takes the URL of DuckStation's
  MCP server (`http://localhost:2346/mcp`; empty detaches). While attached:
  - Saving an ISO/.bin first shuts the emulator down, releasing the disc image lock.
  - **F8** saves if needed, then reboots the `.bin` in the same DuckStation process, so Cheat Engine stays attached.
- **Goto new label:** `Ctrl+Shift+L` in the opcode list inserts a `Goto label` jump to a new label, plus its `Label` opcode.
- **Scoped search:** `Ctrl+Shift+F` and `Ctrl+Shift+R` open Find on *Scripts > Text* / *Scripts > Run* with *Current map* selected.

## Shortcuts

| Key | Action |
| --- | --- |
| `F8` | Run FF7 / reload in DuckStation |
| `Ctrl+F` | Find |
| `Ctrl+Shift+F` | Find text in current map |
| `Ctrl+Shift+R` | Find run in current map |
| `Ctrl+T` | Text manager |
| `Ctrl+G` | Variable manager |
| `Ctrl+B` | Background manager |

Opcode list:

| Key | Action |
| --- | --- |
| `Ctrl++` | Add opcode |
| `Ctrl+Shift+L` | Add goto new label |
| `Enter` | Edit |
| `Delete` | Delete |
| `Ctrl+X` / `Ctrl+C` / `Ctrl+V` | Cut / copy / paste |
| `Ctrl+Shift+C` | Copy text |
| `Shift+Up` / `Shift+Down` | Move opcode |
| `Ctrl+Z` / `Ctrl+Y` | Undo / redo |

## Building

### Windows (one step)

Needs Git, Visual Studio 2022 (or Build Tools) with the C++ x64 workload, CMake, and a Qt 6 MSVC kit
(not in vcpkg, install it with the Qt installer). zlib and ff7tk are fetched and built through the
vcpkg submodule, so the first build needs network access and takes a while.

```bat
set QT_ROOT=C:\Qt\6.x.x\msvc2022_64
build.bat
run.bat
```

`build.bat` builds into `.build` and collects the exe, DLLs and Qt plugins into `.dist\app`; `run.bat` starts it from there.

### Other platforms / manual

Requirements: CMake, Qt 6.2+ (with Network), [zlib](http://www.zlib.net/), [ff7tk](https://www.github.com/sithlord48/ff7tk).

```sh
cmake -S . -B .dist/build -DCMAKE_INSTALL_PREFIX=.dist/install -DCMAKE_BUILD_TYPE=Release
cmake --build .dist/build --config Release
cmake --build .dist/build --target install
```

On Windows, use the MSVC 2022 64-bit Qt kit from a Visual Studio developer prompt (or open the folder in
Qt Creator / Visual Studio). On Debian/Ubuntu: `apt-get install build-essential cmake qt6-base-dev qt6-tools-dev`.
On macOS: `brew install cmake qt6`.

## Contributing

Issues and PRs welcome, ideally upstream at [myst6re/makoureactor](https://github.com/myst6re/makoureactor).
