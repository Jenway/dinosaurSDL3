#include "src/main.h"

int main(int argc, char *argv[])
{
    int flag = 1;
    initBak();
    // The window is open: could enter program loop here (see SDL_PollEvent())

    dinoStatus = 0;

    int quit = 1;
    SDL_Event event;

    do
    {
        paintevent();

        SDL_PollEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            SDL_Log("Event type is %d", event.type);
            quit = 0;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_UP)
            {
                SDL_Log("Event type is %d", event.type);
                dinoJumpHeight = 300;
            }
            else if (event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_DOWN)
            {
                SDL_Log("Event type is %d", event.type);
                dinoJumpHeight = 200;
            }
        default:
            // SDL_Log("Event type is %d", event.type);
            break;
        }
    } while (quit);

    /* Make sure to eventually release the surface resource */
    SDL_FreeSurface(background);
    SDL_FreeSurface(message);

    // destory renderer
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
    }

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}

SDL_Surface *LoadImage(const char *file)
{
    SDL_Surface *surface = SDL_GetWindowSurface(window);

    // 临时的空间，用于存储刚刚加载好的图像
    SDL_Surface *loadedImage = NULL;

    // 优化后的图像，实际使用的是这个图像
    SDL_Surface *optimizedImage = NULL;

    loadedImage = SDL_LoadBMP(file);

    // 如果加载图片没有出错
    if (loadedImage != NULL)
    {
        // 创建一个优化了的图像
        // optimizedImage = SDL_DisplayFormat(loadedImage);
        optimizedImage = SDL_ConvertSurface(loadedImage, surface->format, 0);
        SDL_FreeSurface(loadedImage);
        return optimizedImage;
    }
    else
        printf("Failed to load image at %s: %s\n", file, SDL_GetError());

    // 返回优化后的表面
    return optimizedImage;
}

void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination)
{
    // 新建一个临时的矩形来保存偏移量
    SDL_Rect offset;

    // 将传入的偏移量保存到矩形中
    offset.x = x;
    offset.y = y;
    // 执行表面的Blit
    SDL_BlitSurface(source, NULL, destination, &offset);
}

void apply_dino(int x, int y, int x2, int y2, int w, int h, SDL_Surface *source, SDL_Surface *destination)
{
    // 新建一个临时的矩形来保存偏移量
    SDL_Rect offset;
    // 切割原图
    SDL_Rect cutRect;

    // 将传入的偏移量保存到矩形中
    offset.x = x;
    offset.y = y;
    cutRect.x = x2;
    cutRect.y = y2;
    cutRect.w = w;
    cutRect.h = h;
    // 执行表面的Blit
    SDL_BlitSurface(source, &cutRect, destination, &offset);
}

void initBak()
{

    SDL_Init(SDL_INIT_VIDEO); // Initialize SDL2

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "Hello,dinosaur",       // window title
        SDL_WINDOWPOS_CENTERED, // initial x position
        SDL_WINDOWPOS_CENTERED, // initial y position
        WIDTH,                  // width, in pixels
        HEIGHT,                 // height, in pixels
        SDL_WINDOW_SHOWN
        //  | SDL_WINDOW_FULLSCREEN_DESKTOP // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL)
    {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
    }

    /* We must call SDL_CreateRenderer in order for draw calls to affect this window. */
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    // 设置背景
    background = LoadImage(BAKIMG_PATH);
    // 设置恐龙
    message = LoadImage(DINO_PATH);

    surface = SDL_GetWindowSurface(window);
}
void dino_run()
{
    // SDL_FillRect(message, NULL, 0x000000);
    apply_dino(200, HEIGHT - dinoJumpHeight, dinoRun[dinoStatus], 0, 87, 94, message, surface);
    // 更新窗口并显示
    SDL_UpdateWindowSurface(window);
}
void paintevent()
{
    if (dinoStatus == 2)
        dinoStatus = 0;

    apply_surface(0, 0, background, surface);
    dino_run();

    dinoStatus++;
}