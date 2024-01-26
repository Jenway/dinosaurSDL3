#include <Data.h>
#include <Runner.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#include <stdbool.h>

// Starts up SDL and creates window
bool init_SDL();
void close_SDL();

// callback function
Uint32 callback(Uint32 interval, void* param);

// The window we'll be rendering to
SDL_Window* gWindow = NULL;
// The window renderer
SDL_Renderer* gRenderer = NULL;

Runner* this = NULL;

int main(int argc, char* argv[])
{
    if (!init_SDL()) {
        SDL_Log("Failed to initialize SDL!\n");
    } else {
        this = Runner_constructor(gWindow, gRenderer);
        if (this == NULL) {
            SDL_Log("Failed to initialize Runner!\n");
        } else {

            // load resources
            this->loadImages(this);
            this->loadAudio(this);
            this->init(this);
            this->loop(this);
        }
        // free resources
        this->destructor(this);
    }
    // Free resources and close SDL
    close_SDL();
    return 0;
}

Uint32 callback(Uint32 interval, void* param) // 回调函数
{
    // SDL_Log("done %d\n", interval);
    Runner* this = param;
    this->paintEvent(this);
    /*
     * WARNING: The rendering in SDL only happens in the main thread.
     * SDL/OpenGL requires all rendering calls to come exactly from 1 thread.
     * The thread that created the SDL_Renderer
     */
    return interval;
}

bool init_SDL()
{
    // Initialization flag
    bool success = true;

    int SDL_Init_Flag = SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS;
    // Initialize SDL
    if (SDL_Init(SDL_Init_Flag) != 0) {
        // Q: Why not use SDL_InitSubSystem()?
        // A: SDL_InitSubSystem() and SDL_Init() are interchangeable in SDL.
        SDL_Log("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    } else {
        // Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            SDL_Log("Warning: Linear texture filtering not enabled!");
        }
        // Create window
        gWindow = SDL_CreateWindow(SCREEN_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
        if (gWindow == NULL) {
            SDL_Log("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            Uint32 SDL_CreateRenderer_Flag = SDL_RENDERER_SOFTWARE | SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
            // Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, "opengl", SDL_CreateRenderer_Flag);
            if (gRenderer == NULL) {
                SDL_Log("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            } else {
                // Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                // Initialize PNG && JPG loading from SDL_image
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    SDL_Log("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

void close_SDL()
{
    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}