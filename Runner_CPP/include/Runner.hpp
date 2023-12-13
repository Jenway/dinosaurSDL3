#include <ITexture.hpp>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL_video.h>
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
    ITexture imageSprite;

private:
    TRex trex;

private:
    bool running;

private:
    void mainLoop();
    void handleEvent(SDL_Event& event);
    void update();
    void render();
};
