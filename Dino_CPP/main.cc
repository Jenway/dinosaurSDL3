
#include <SDL_keycode.h>
#define SDL_MAIN_USE_CALLBACKS
#include <Horizon.hpp>
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <SDL_error.h>
#include <SDL_video.h>
#include <Sprite.hpp>
#include <Utils.hpp>
#include <string>
#include <tRex.hpp>

bool crashed = false;
bool paused = false;
bool playing = false;
float currentSpeed { INITIAL_SPEED };

SDL_Window* window;
SDL_Renderer* renderer;
Sprite* sprite;
TRex* trex;
Horizon* horizon;

static constexpr auto ThrowError = [](std::string msg = {}) {
    if (msg.empty()) {
        msg = SDL_GetError();
    } else {
        msg += SDL_GetError();
    }
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", msg.c_str());
};

auto reset = []() {
    trex->reset();
    horizon->clear();
    SDL_RenderClear(renderer);
    SDL_AppIterate();
    crashed = false;
    paused = false;
    playing = false;
};

auto play = []() {
    if (!crashed) {
        paused = false;
        playing = true;
    } else {
        reset();
        playing = false;
    }
};

int SDL_AppInit(int argc, char** argv)
{
    static constexpr const char* SCREEN_TITLE = "SDL Runner";
    static constexpr int Init_Flag = SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS;
    static constexpr Uint32 SDL_CreateRenderer_Flag = SDL_RENDERER_SOFTWARE | SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    static constexpr int SDL_IMG_Init_Flag = IMG_INIT_PNG;
    if (SDL_Init(Init_Flag) != 0) {
        ThrowError();
        return -1;
    }
    // Set texture filtering to linear
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        ThrowError();
        return -1;
    }

    // Create window
    window = SDL_CreateWindow(SCREEN_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        ThrowError("Window could not be created! SDL Error: ");
        return -1;
    }

    // Create renderer for window
    renderer = SDL_CreateRenderer(window, "opengl", SDL_CreateRenderer_Flag);

    if (renderer == nullptr) {
        ThrowError("Renderer could not be created! SDL Error: ");
        return -1;
    }

    // Initialize renderer color
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Initialize PNG loading
    if (!(IMG_Init(SDL_IMG_Init_Flag) & SDL_IMG_Init_Flag)) {
        std::string msg { "SDL_image could not initialize! SDL_image Error: " };
        msg += IMG_GetError();
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", msg.c_str());
        return -1;
    }

    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        std::string msg { "Warning: Linear texture filtering not enabled!" };
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", msg.c_str());
    }

    if (sprite = new Sprite(renderer, ASSETS_PATH); sprite == nullptr) {
        ThrowError("Sprite could not be created! SDL Error: ");
        return -1;
    }

    if (trex = new TRex(); trex == nullptr) {
        ThrowError("tRex could not be created! SDL Error: ");
        return -1;
    }

    if (horizon = new Horizon(); horizon == nullptr) {
        ThrowError("Horizon could not be created! SDL Error: ");
        return -1;
    }

    return 0;
}

int SDL_AppIterate(void)
{

    if (playing) {

        trex->update(2, currentSpeed);
        horizon->update(2, currentSpeed);
        if (horizon->checkCollision(trex->getCollisionBox())) {
            crashed = true;
            playing = false;
            trex->mStatus = TRex::Status::kCrashed;
        }
    }
    SDL_RenderClear(renderer);
    horizon->draw(*sprite);
    sprite->draw(*trex);
    SDL_RenderPresent(renderer);

    return 0;
}

int SDL_AppEvent(const SDL_Event* event)
{

    auto onKeyUp = [&](const SDL_Event* event) {
        bool isJumpKey = event->key.keysym.sym == SDLK_SPACE;

        if (trex->mStatus == TRex::Status::kRunning && isJumpKey) {
            trex->endJump();
        } else if (trex->mStatus == TRex::Status::kDucking) {
            trex->setSpeedDrop(false);
            trex->setDuck(false);
        } else if (crashed) {

        } else if (paused && isJumpKey) {
            // trex->reset();
            // play();
        }
    };

    switch (event->type) {
    case SDL_EVENT_QUIT:
        return 1;
    case SDL_EVENT_KEY_UP:
        onKeyUp(event);
        break;
    case SDL_EVENT_KEY_DOWN: {
        auto keyType = event->key.keysym.sym;
        if (keyType == SDLK_ESCAPE) {
            return 1;
        }

        if (!crashed && !paused) {
            if (!playing) {
                playing = true;
            }
            if (keyType == SDLK_SPACE || keyType == SDLK_UP) {
                if (trex->mStatus != TRex::Status::kJumping && trex->mStatus != TRex::Status::kDucking) {
                    trex->startJump(currentSpeed);
                }
            } else if (keyType == SDLK_DOWN) {
                if (playing) {
                    if (trex->mStatus == TRex::Status::kJumping) {
                        trex->setSpeedDrop(true);
                    } else if (trex->mStatus == TRex::Status::kRunning) {
                        trex->setDuck(true);
                    }
                }
            }
        } else if (crashed) {
            if (keyType == SDLK_SPACE || keyType == SDLK_UP) {
                reset();
            }
        } else if (paused) {
            if (keyType == SDLK_SPACE || keyType == SDLK_UP) {
                play();
            }
        }
    } break;
    default:
        break;
    }

    return 0;
}

void SDL_AppQuit(void)
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();

    delete sprite;
    delete trex;
    delete horizon;
}