# UltraSmashEngine4k & Koopa Engine — Setup Guide

Setup and run the platform fighter engines: **UltraSmashEngine4k** and **Koopa Engine**. Single C++ files, SDL2, optional assets (sprites, music).

| Engine | Source file | Binary |
|--------|-------------|--------|
| **UltraSmashEngine4k** | `ultrasmashengine4k.cpp` | `ultrasmashengine4k` |
| **Koopa Engine** | `#KOOPAENGINE.cpp` | `koopaengine` |

---

## Prerequisites

- **C++17** compiler (`g++` or Clang)
- **SDL2** (development headers and library)

### Install SDL2

| Platform   | Command |
|-----------|--------|
| **macOS** | `brew install sdl2` |
| **Ubuntu/Debian** | `sudo apt install libsdl2-dev` |
| **Fedora** | `sudo dnf install SDL2-devel` |
| **Windows** | Use [vcpkg](https://vcpkg.io) or [MSYS2](https://www.msys2.org): `pacman -S mingw-w64-ucrt-x86_64-SDL2` |

---

## Project layout

Keep the source files and (optionally) assets in one place:

```
/Volumes/1TB/:CODING/
├── ultrasmashengine4k.cpp   # UltraSmashEngine4k source
├── #KOOPAENGINE.cpp         # Koopa Engine source
├── SETUP.md                 # This file
├── assets/
│   └── sprites/
│       └── fighters.bmp     # Optional: 32×32 sprite sheet (shared)
└── music/                   # Optional: for future OST (SDL2_mixer)
    └── menu.ogg
```

Binaries are produced in the same directory as the source (or wherever you run the build from).

---

## Build

From the project directory. Use **one** of the following depending on which engine you want.

### UltraSmashEngine4k

```bash
cd /Volumes/1TB/:CODING

g++ -std=c++17 -O2 -o ultrasmashengine4k ultrasmashengine4k.cpp $(pkg-config --cflags --libs sdl2)
```

- **Binary:** `./ultrasmashengine4k` (Unix) or `ultrasmashengine4k.exe` (Windows)

### Koopa Engine

```bash
cd /Volumes/1TB/:CODING

g++ -std=c++17 -O2 -o koopaengine "#KOOPAENGINE.cpp" $(pkg-config --cflags --libs sdl2)
```

- **Binary:** `./koopaengine` (Unix) or `koopaengine.exe` (Windows)
- The `#` in the filename must be quoted in the shell (`"#KOOPAENGINE.cpp"`).

### Build notes

- **macOS/Linux:** `pkg-config` is usually present when SDL2 is installed.
- **Windows (MinGW):** You may need to pass include/lib paths and `-lSDL2` manually instead of `pkg-config`.

---

## Run

From the **project directory** (so `assets/` is found next to the binary).

**UltraSmashEngine4k:**

```bash
./ultrasmashengine4k
```

**Koopa Engine:**

```bash
./koopaengine
```

If you run from elsewhere, the program will still start but will not find `assets/sprites/fighters.bmp` and will use colored-rectangle placeholders for fighters.

---

## Optional: Sprite sheet (SMBZ-style)

Fighter sprites are loaded from:

- **Path:** `assets/sprites/fighters.bmp`
- **Format:** BMP (SDL2 built-in; no extra libs)
- **Grid:** 32×32 pixels per frame, row-major

Suggested frame layout:

| Frame | 0   | 1–3    | 4   | 5   | 6    |
|-------|-----|--------|-----|-----|------|
| Use   | Idle| Walk   | Jump| Fall| Attack |

If the file is missing or fails to load, the engine draws colored rectangles for P1/P2.

---

## Optional: Music (OST)

Background music is **not** implemented by default. To add it:

1. Link **SDL2_mixer** and implement `SoundEngine::play_music_ost()` (e.g. `Mix_LoadMUS`, `Mix_PlayMusic(..., -1)`).
2. Place your track(s) in e.g. `music/` and pass the path from code (e.g. `music/menu.ogg`).

SFX (menu, hit, jump, KO, result) are procedural and work without any asset files.

---

## Controls

| Context   | Action        | Keys |
|----------|---------------|------|
| **Menu** | Move cursor   | ↑ / ↓ |
|          | Confirm       | Enter / Space |
|          | Exit game     | Select "EXIT GAME" and confirm |
| **Character select** | Move | ← → ↑ ↓ |
|          | Confirm       | Enter / Space |
| **Stage select**     | Move | ← → |
|          | Start fight   | Enter / Space |
| **Fight**| P1 move       | ← → |
|          | P1 jump       | ↑ |
|          | P1 attack     | Z |
|          | P2 move       | A / D |
|          | P2 jump       | W |
|          | P2 attack     | G |
|          | Quit to menu  | Esc |
| **Result**           | Back to menu | Enter / Space / Esc |

---

## Flow

1. **Boot** — Splash and credits screens.
2. **Intro** — Title then hold.
3. **Main menu** — 1 Player, 2 Player, Versus, Options, Exit.
4. **Character select** — Grid of 12 fighters; confirm (1P: once, 2P/Versus: P1 then P2).
5. **Stage select** — List of stages; confirm to start fight.
6. **Fight** — Two players on one platform, stocks and percent; first to lose all stocks loses.
7. **Result** — Winner screen; return to main menu.

---

## Troubleshooting

| Issue | Fix |
|-------|-----|
| `SDL_Init` / SDL not found | Install SDL2 dev package and ensure `pkg-config --libs sdl2` works. |
| No sprites, only rectangles | Create `assets/sprites/` and add `fighters.bmp`, or run from the project directory. |
| Black screen or no window | Check that the SDL2 video driver works (e.g. no headless environment without display). |
| Binary not found | Run the build from the same directory where you want the binary (or adjust `-o` path). |
| Koopa Engine: "No such file" when building | Quote the source name: `"#KOOPAENGINE.cpp"` (the `#` is special in the shell). |

---

## One-shot setup (copy-paste)

**UltraSmashEngine4k:**

```bash
cd /Volumes/1TB/:CODING
mkdir -p assets/sprites
g++ -std=c++17 -O2 -o ultrasmashengine4k ultrasmashengine4k.cpp $(pkg-config --cflags --libs sdl2)
./ultrasmashengine4k
```

**Koopa Engine:**

```bash
cd /Volumes/1TB/:CODING
mkdir -p assets/sprites
g++ -std=c++17 -O2 -o koopaengine "#KOOPAENGINE.cpp" $(pkg-config --cflags --libs sdl2)
./koopaengine
```

After this, add `assets/sprites/fighters.bmp` (32×32 grid) if you want in-game sprites. Both engines use the same assets path.
