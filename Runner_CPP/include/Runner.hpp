#if !defined(__RUNNER_HPP__)
#define __RUNNER_HPP__

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL_video.h>
#include <Sprite.hpp>
#include <tRex.hpp>

class Runner {
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

private:
    bool playing = false;
    bool crashed = false;
    bool paused = false;
    double currentSpeed = 24.0;

private:
    constexpr static int FPS = 60;
    constexpr static const char* const ASSETS_PATH = "assets/sprite.png";

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