#include "src/main.h"

int main(int argc, char *argv[])
{
    int flag = 1;
    initBak();
    // The window is open: could enter program loop here (see SDL_PollEvent())

    dinoStatus = 0;
    int quit = 1;
    SDL_Event event;
    paintevent();
    while (gameStart == 1)
    {
        SDL_PollEvent(&event);
        keyarr = SDL_GetKeyboardState(NULL);
        switch (event.type)
        {
        case SDL_QUIT:
            SDL_Log("Event type is %d", event.type);
            gameEnd();
            break;
        default:
            break;
        }
        if (keyarr[SDL_SCANCODE_SPACE] == 1 || keyarr[SDL_SCANCODE_W] || keyarr[SDL_SCANCODE_UP])
        {
            SDL_Log("w is pressed!");
            dinoStatus = 2;
            gameStart = 0;
        }
        else if (keyarr[SDL_SCANCODE_ESCAPE] == 1)
        {
            quit = 0;
        }
    }

    while (quit == 1)
    {

        if (dinoStatus == 4)
        {

            if (sitCache < SITFPS)
            {
                sitCache += 1;
            }

            else
            {
                sitCache = 0;
                dinoStatus = 1;
            }
        }
        timeCountPre = SDL_GetTicks();

        keyarr = SDL_GetKeyboardState(NULL);
        if ((keyarr[SDL_SCANCODE_SPACE] == 1 || keyarr[SDL_SCANCODE_W] || keyarr[SDL_SCANCODE_UP]) && dinoStatus == 1)
        {
            SDL_Log("w2 is pressed!");
            dinoStatus = 2;
        }
        else if ((keyarr[SDL_SCANCODE_S] == 1 || keyarr[SDL_SCANCODE_DOWN]) && dinoStatus == 1)
        {
            dinoStatus = 4;
        }
        else if (keyarr[SDL_SCANCODE_ESCAPE] == 1)
        {
            quit = 0;
        }

        SDL_PollEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            SDL_Log("Event type is %d", event.type);
            quit = 0;
            break;
        default:
            break;
        }
        timeCountCur = SDL_GetTicks();

        paintevent();
        SDL_Delay(FramePerSecond - (timeCountCur - timeCountPre));
    }

    gameEnd();
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
    // background = LoadImage(BAKIMG_PATH);
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
        if (runCache >=RUNFPS)
        {
            dinoRunningStatus++;
            runCache =0;
        }
        else
        {
            runCache ++;
        }
        
        
        
    }
    else if (dinoStatus == 0)
    {
        apply_dino(20, HEIGHT + FOOTHEIGHT - dinoJumpHeight, dinoRun[2], 0, 87, 94, message, surface);
    }

    else if (dinoStatus == 2 || dinoStatus == 3)
    {
        apply_dino(20, HEIGHT + FOOTHEIGHT - dinoJumpHeight, dinoRun[2], 0, 87, 94, message, surface);
    }
    else if (dinoStatus == 4)
    {
        if (dinoRunningStatus >= 2)
        {
            dinoRunningStatus = 0;
        }
        // SDL_FillRect(message, NULL, 0x000000);
        apply_dino(20, HEIGHT + FOOTHEIGHT - dinoJumpHeight, dinoRun[dinoRunningStatus + 3], 0, 118, 94, message, surface);
        dinoRunningStatus++;
    }
}
void paintevent()
{

    // apply_surface(0, 0, background, surface);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer); // 清空渲染器
    if (dinoStatus != 0)
    {
        drawCloud();
        drawRoad();
        drawPlant();
    }

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
    apply_dino(0, HEIGHT - 25, roadStatus, 104, 800, 30, message, surface);
}

void drawCloud()
{

    if (cloudStaatus >= -800)
    {
        cloudStaatus -= ROADSPEED / 2;
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

void drawPlant()
{

        if (plantStatus >= -8000)
        {
            plantStatus -= ROADSPEED;
        }
        else
        {
            plantStatus = 0;
        } /* code */

    apply_dino(plantStatus, HEIGHT -PLANTHEIGHT, 446, 2, 28, 70, message, surface); 
    apply_dino(plantStatus+PLANTLENGTH, HEIGHT-PLANTHEIGHT, 480, 2, 28, 70, message, surface);
    apply_dino(plantStatus+4*PLANTLENGTH, HEIGHT-PLANTHEIGHT, 514, 2, 28, 70, message, surface);
    apply_dino(plantStatus+10*PLANTLENGTH, HEIGHT-PLANTHEIGHT, 548, 2, 28, 70, message, surface);
    apply_dino(plantStatus+11*PLANTLENGTH, HEIGHT-PLANTHEIGHT, 582, 2, 28, 70, message, surface); 
    apply_dino(plantStatus+14*PLANTLENGTH, HEIGHT-PLANTHEIGHT, 616, 2, 28, 70, message, surface);
    apply_dino(plantStatus+18*PLANTLENGTH, HEIGHT-PLANTHEIGHT, 648, 2, 28, 70, message, surface);


    apply_dino(plantStatus+20*PLANTLENGTH, HEIGHT-PLANTHEIGHT-30, 652, 2, 50, 100, message, surface);
    apply_dino(plantStatus+9*PLANTLENGTH, HEIGHT-PLANTHEIGHT-30, 702, 2, 50, 100, message, surface);
    apply_dino(plantStatus+16*PLANTLENGTH, HEIGHT-PLANTHEIGHT-30, 752, 2, 50, 100, message, surface); // 10
    apply_dino(plantStatus+20*PLANTLENGTH, HEIGHT-PLANTHEIGHT-30, 802, 2, 50, 100, message, surface);
}
void dinoJump()
{
    switch (dinoStatus)
    {
    case 2:
        if (dinoJumpHeight < DINOFINALHEIGHT)
        {
            dinoJumpHeight += ROADSPEED * 2;
        }
        else
        {
            dinoStatus = 3;
        }
        break;
    case 3:
        if (dinoJumpHeight - ROADSPEED > 200)
        {
            dinoJumpHeight -= ROADSPEED * 2;
        }
        else
        {
            dinoJumpHeight = 200;
            dinoStatus = 1;
        }
        break;
    default:
        break;
    }
}

void gameEnd()
{

    /* Make sure to eventually release the surface resource */
    // SDL_FreeSurface(background);
    SDL_FreeSurface(message);

    // destory renderer

    SDL_DestroyRenderer(renderer);

    // Close and destroy the window
    SDL_DestroyWindow(window);
    // Clean up
    SDL_Quit();
}