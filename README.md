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

### Windows

Needs Git, Visual Studio 2022 with the C++ x64 tools, and a Qt 6 MSVC kit (install via the Qt installer;
it is not in vcpkg). zlib and ff7tk are built from the `vcpkg` submodule, so the first build needs network
and takes a while.

```bat
set QT_ROOT=C:\Qt.x.x\msvc2022_64
build.bat
run.bat
```

`build.bat` builds into `.build` and collects the exe, DLLs and Qt plugins into `.distpp`. `run.bat` launches it.

### Linux / macOS

Needs CMake, Qt 6.2+ (with Network), [zlib](http://www.zlib.net/) and [ff7tk](https://www.github.com/sithlord48/ff7tk).
Debian/Ubuntu: `apt-get install build-essential cmake qt6-base-dev qt6-tools-dev`. macOS: `brew install cmake qt6`.

```sh
cmake -S . -B .build -DCMAKE_BUILD_TYPE=Release
cmake --build .build
```

## Contributing

Issues and PRs welcome, ideally upstream at [myst6re/makoureactor](https://github.com/myst6re/makoureactor).
