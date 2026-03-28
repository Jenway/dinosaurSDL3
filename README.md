# Dino C

A reconstructed SDL3 branch of the old Chrome Dino clone.

This branch keeps the old SDL2 history on `main`, but moves the active project
layout to a pixi-managed SDL3 build that uses `pkg-config` instead of vendored
SDK files.

## Build

This stage only introduces the project scaffold and native build tooling.
Gameplay code is imported in follow-up commits.

## Layout

- `Resources/`: sprite assets
- `third_party/`: vendored third-party source
- `include/`: project headers
- `src/`: project sources
- `tests/`: regression tests
