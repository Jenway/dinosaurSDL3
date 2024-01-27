#if !defined(__RUNNER_HPP__)
#define __RUNNER_HPP__

#include <Horizon.hpp>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL_video.h>
#include <Sprite.hpp>
#include <tRex.hpp>

class Runner {
private:
    constexpr static int FPS = 60;
    constexpr static double INITIAL_SPEED = 8 * RATE;
    constexpr static const char* const ASSETS_PATH = "assets/sprite.png";

public:
    Runner() = delete;
    Runner(const SDL_Window& window, SDL_Renderer* renderer);
    ~Runner() = default;
    void run();

private:
    const SDL_Window& window;
    SDL_Renderer* renderer;
    Sprite imageSprite;
    bool RUNNING_FLAG = false;

private:
    TRex trex;
    Horizon horizon;

private:
    bool playing = false;
    bool crashed = false;
    bool paused = false;
    double currentSpeed { INITIAL_SPEED };

private:
    Uint32 time = 0;

private:
    void play();
    void reset();
    void mainLoop();
    void handleEvent(SDL_Event& event);
    void update(Uint32 frameTime);
    void render();

private:
    void debugTime()
    {
    }
};

#endif // __RUNNER_HPP__s