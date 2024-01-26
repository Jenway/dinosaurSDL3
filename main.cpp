#include <Runner.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <SDL_video.h>
#include <memory>
#include <stdexcept>

class SDL_Application {
public:
    SDL_Application();
    ~SDL_Application();
    void run();

private:
    void init();
    void clean();

private:
    static constexpr int SCREEN_WIDTH = 1240;
    static constexpr int SCREEN_HEIGHT = 500;
    static constexpr const char* SCREEN_TITLE = "SDL Runner";
    static constexpr int Init_Flag = SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS;
    static constexpr Uint32 SDL_CreateRenderer_Flag = SDL_RENDERER_SOFTWARE | SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    static constexpr int SDL_IMG_Init_Flag = IMG_INIT_PNG;

private:
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;
    std::unique_ptr<Runner> runner;

    bool running;
};

int main(int argc, char* argv[])
{
    SDL_Application app;
    app.run();
    return 0;
}

SDL_Application::SDL_Application()
    : window(nullptr, SDL_DestroyWindow)
    , renderer(nullptr, SDL_DestroyRenderer)
    , running(true)
{
    try {
        this->init();
    } catch (const std::exception& e) {
        SDL_Log("Failed to initialize SDL: %s", e.what());
        throw;
    } catch (...) {
        SDL_Log("Failed to initialize SDL: unknown error");
        throw;
    }
    this->runner.reset(new Runner(*this->window, this->renderer.get()));
}

SDL_Application::~SDL_Application()
{
    this->clean();
}

void SDL_Application::run()
{
    this->runner->run();
}

void SDL_Application::init()
{
    // Initialize SDL

    if (SDL_Init(this->Init_Flag) != 0) {
        throw std::runtime_error(SDL_GetError());
    }
    // Set texture filtering to linear
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        throw std::runtime_error(SDL_GetError());
    }

    // Create window
    this->window.reset(SDL_CreateWindow(SCREEN_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE));
    if (this->window == nullptr) {
        throw std::runtime_error("Window could not be created! SDL Error: " + std::string(SDL_GetError()));
    }

    // Create renderer for window
    this->renderer.reset(SDL_CreateRenderer(this->window.get(), "opengl", this->SDL_CreateRenderer_Flag));

    if (this->renderer == nullptr) {
        throw std::runtime_error("Renderer could not be created! SDL Error: " + std::string(SDL_GetError()));
    }

    // Initialize renderer color
    SDL_SetRenderDrawColor(this->renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);

    // Initialize PNG loading
    if (!(IMG_Init(this->SDL_IMG_Init_Flag) & this->SDL_IMG_Init_Flag)) {
        throw std::runtime_error("SDL_image could not initialize! SDL_image Error: " + std::string(IMG_GetError()));
    }
}

void SDL_Application::clean()
{
    this->renderer.reset();
    this->window.reset();

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}