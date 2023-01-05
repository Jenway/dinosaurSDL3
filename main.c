#include "src/main.h"

int main(int argc, char *argv[])
{
    int flag = 1;
    initBak();
    // The window is open: could enter program loop here (see SDL_PollEvent())

    dinoStatus = 1;

    int quit = 1;
    SDL_Event event;

    do
    {
        Uint64 start = SDL_GetPerformanceCounter();

        paintevent();

        SDL_PollEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            SDL_Log("Event type is %d", event.type);
            quit = 0;
            break;
        case SDL_KEYDOWN:
            if (dinoStatus != 3 && (event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_UP))
            {
                SDL_Log("Event type is %d", event.type);
                dinoStatus = 2;
            }
            else if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                SDL_Log("Event type is %d", event.type);
                quit = 0;
            }

            else if (event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_DOWN)
            {
                SDL_Log("Event type is %d", event.type);
            }
            break;
        default:
            // SDL_Log("Event type is %d", event.type);
            break;
        }
        // SDL_Log("CLOUDS: %d",cloudStaatus);

        Uint64 end = SDL_GetPerformanceCounter();

        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        // SDL_Log("current FPS,%f.6", 16.666f - elapsedMS);
        // Cap to 60 FPS
        SDL_Delay((float)(16.666f - elapsedMS));

    } while (quit == 1);

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
    texture = SDL_CreateTextureFromSurface(renderer, source);

    cutset.x = 0;
    cutset.y = 0;
    cutset.w = WIDTH;
    cutset.h = HEIGHT;
    // 将传入的偏移量保存到矩形中
    offset.x = 0;
    offset.y = 0;
    offset.w = WIDTH;
    offset.h = HEIGHT;
    // 执行表面的Blit
    // SDL_BlitSurface(source, &cutset, destination, &offset);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texture, &cutset, &offset);
}

void apply_dino(int x, int y, int x2, int y2, int w, int h, SDL_Surface *source, SDL_Surface *destination)
{
    // 将传入的偏移量保存到矩形中
    offset.x = x;
    offset.y = y;
    offset.w = w;
    offset.h = h;

    cutRect.x = x2;
    cutRect.y = y2;
    cutRect.w = w;
    cutRect.h = h;
    // 执行表面的Blit
    // SDL_BlitSurface(source, &cutRect, destination, &offset);
    SDL_RenderCopy(renderer, texture, &cutRect, &offset);
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
        SDL_WINDOW_SHOWN);

    // Check that the window was successfully created
    if (window == NULL)
    {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
    }

    /* We must call SDL_CreateRenderer in order for draw calls to affect this window. */
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // 设置背景
    background = LoadImage(BAKIMG_PATH);
    // 设置恐龙
    message = LoadImage(DINO_PATH);
    texture = SDL_CreateTextureFromSurface(renderer, message);

    // surface = SDL_GetWindowSurface(window);
}
void dino_run()
{
    if (dinoStatus == 1)
    {
        if (dinoRunningStatus >= 2)
        {
            dinoRunningStatus = 0;
        }
        // SDL_FillRect(message, NULL, 0x000000);
        apply_dino(20, HEIGHT + FOOTHEIGHT - dinoJumpHeight, dinoRun[dinoRunningStatus], 0, 87, 94, message, surface);
        dinoRunningStatus++;
    }
    else if (dinoStatus == 2 || dinoStatus == 3)
    {
        apply_dino(20, HEIGHT + FOOTHEIGHT - dinoJumpHeight, dinoRun[2], 0, 87, 94, message, surface);
    }
}
void paintevent()
{

    // apply_surface(0, 0, background, surface);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer); // 清空渲染器
    drawCloud();
    drawRoad();
    if (dinoStatus == 2 || dinoStatus == 3)
    {
        dinoJump();
    }

    dino_run();
    // apply_dino(20, 200, dinoRun[2], 0, 87, 94, message, NULL);
    // 更新窗口并显示
    // SDL_UpdateWindowSurface(window);
    SDL_RenderPresent(renderer);
}

void drawRoad()
{
    if (dinoStatus != 0)
    {
        if (roadStatus < 1600)
        {
            roadStatus += ROADSPEED;
        }
        else
        {
            roadStatus = 0;
        } /* code */
    }

    // SDL_FillRect(message, NULL, 0x000000);
    apply_dino(0, HEIGHT - 25, roadStatus, 104, 2000, 30, message, surface);
    // 更新窗口并显示
    SDL_UpdateWindowSurface(window);
}

void drawCloud()
{

    if (cloudStaatus >= -800)
    {
        cloudStaatus -= ROADSPEED;
    }
    else
    {
        cloudStaatus = 1500;
    }

    apply_dino(cloudStaatus, CLOUDHEIGHT, 165, 2, 93, 27, message, surface);
    apply_dino(cloudStaatus + 800, CLOUDHEIGHT + 30, 165, 2, 93, 27, message, surface);
    apply_dino(cloudStaatus + 600, CLOUDHEIGHT + 60, 165, 2, 93, 27, message, surface);
    apply_dino(cloudStaatus - 400, CLOUDHEIGHT + 50, 165, 2, 93, 27, message, surface);
    apply_dino(cloudStaatus - 300, CLOUDHEIGHT - 50, 165, 2, 93, 27, message, surface);
    apply_dino(cloudStaatus - 700, CLOUDHEIGHT - 50, 165, 2, 93, 27, message, surface);
}

void dinoJump()
{
    switch (dinoStatus)
    {
    case 2:
        if (dinoJumpHeight < 400)
        {
            dinoJumpHeight += 10;
        }
        else
        {
            dinoStatus = 3;
        }
        break;
    case 3:
        if (dinoJumpHeight > 200)
        {
            dinoJumpHeight -= 10;
        }
        else
        {
            dinoStatus = 1;
        }
        break;
    default:
        break;
    }
}