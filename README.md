# Dino C

A reconstructed SDL3 branch of the old Chrome Dino clone.

This branch keeps the old SDL2 history on `main`, but moves the active project
layout to a pixi-managed SDL3 build that uses `pkg-config` instead of vendored
SDK files.

## Build

Native desktop build:

```bash
pixi run build
```

## Layout

- `Resources/`: sprite assets
- `third_party/`: vendored third-party source
- `include/`: project headers
- `src/`: project sources
- `tests/`: regression tests

The web build, persistence layer, and regression tests are added in later
reconstruction commits.
