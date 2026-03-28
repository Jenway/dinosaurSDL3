# Dino C

A small Chrome Dino clone written in C with SDL3.

It runs natively on desktop and can also be built for the browser with Emscripten.
The codebase stays intentionally lightweight: plain C, SDL3 renderer, a fixed-timestep game loop, and a small amount of data-driven configuration for obstacles and tuning.

## Features

- SDL3 native build
- Emscripten / WebAssembly build
- Chromium-aligned sprites and obstacle data
- Fixed-timestep update loop with render interpolation
- Local high-score persistence
- Lightweight regression tests for core gameplay logic

## Build

This project uses `pixi` to manage dependencies.

Native build:

```bash
pixi run build
```

Or directly:

```bash
pixi run make dinosaur
```

## Run

Native executable:

```bash
./build/dinosaur.exe
```

## Test

Run the lightweight gameplay regression tests:

```bash
pixi run test
```

## Web Build

Build the wasm version:

```bash
pixi run build-web
```

Serve it locally:

```bash
pixi run serve-web
```

Web high-score persistence currently uses browser `localStorage`.

## Controls

- `Space` / `W` / `Up Arrow`: jump, start, restart
- `S` / `Down Arrow`: duck
- `Esc`: quit
- `F1`: toggle collision boxes
- `F2`: toggle background debug overlay

## Project Layout

- `src/game.c`: gameplay, movement, spawning, scoring, collision
- `src/render.c`: SDL rendering and interpolation
- `src/persistence.c`: native/web high-score persistence
- `src/obstacles.c`: obstacle definitions and collision boxes
- `src/main.c`: SDL app callbacks and main loop orchestration
- `tests/test_game.c`: small regression test suite

## Notes

- Native persistence uses SDL user storage.
- Web persistence uses `localStorage` instead of IndexedDB.
- The build still includes `stb_image`, which may emit third-party warnings during compilation.
- Historical notes for the old SDL2 / monorepo repository are in `docs/history-retrospective.md`.
