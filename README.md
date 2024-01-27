# 小恐龙与 SDL

## 这是什么 ?

一个 idea 来自于 Chrome 的 `t-rex runner` 的 ... 游戏 ?

## 它好玩吗 ?

它不好玩 , 不管是三个版本中的哪个 , 而且都没有做完

## 那为什么还要放在这里 ?

纪念 , 或者说是一种和解 , 我终于放弃了这个项目

## 构建

参考路径下 README.md

- Dino_C: 最早的一个，连同依赖的库 (SDL-2.26.2 , SDL2_image-2.6.2) 和动态链接库一同上传了
- Runner_C: 次晚的一个, 用 `xmake` 构建 , 很方便 , 不过少了运行时环境
- Runner_CPP: 最简陋的一个, 图新鲜用了还没正式发布的 `SDL3` , 直接参考官方仓库构建好 SDL 和 SDL_image 然后 `cmake install` , 就可以了
- Dino_CPP: 试了试 SDL3 新的回调 main 方法，顺便尝试了静态编译，除此之外是一样的简陋

## Release

Release 中有构建好的，有单文件打包，以及自己编译的 SDL3 & SDL3-image devl 包

## License

The code in this repo is under BSD 3-Clause License

当然，我用来参考的官方 JS 代码不是，它们的版权自然属于 Chromium
