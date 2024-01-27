# dinosaurSDL3

小恐龙🦖 made with SDL3

## 说明

- CONTROL YOUR DINOSAUR TO AVOID OBSTACLES AND GET SCORES

- idea 来自于 Chrome 的 `t-rex runner` 游戏

## 控制

- Press the space bar to jump and to start the game

- Use the down arrow key to duck

## 依赖

- SDL3
- SDL3_image
- CMake

## 编译

确保你安装了 `SDL3`

```bash
mkdir build
cmake -S . -B ./build
cmake --build ./build
```

## 运行

由于 SDL 的机制，确保命令执行路径与 `assets` 目录同级