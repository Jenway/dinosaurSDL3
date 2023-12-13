#include <Runner.hpp>
#include <SDL_events.h>
#include <iostream>
Runner::Runner(const SDL_Window& window, SDL_Renderer* renderer)
    : window(window)
    , renderer(renderer)
    , imageSprite(renderer, "assets/sprite.png")
{
}

void Runner::run()
{
    this->running = true;
    this->mainLoop();
}

void Runner::mainLoop()
{
    SDL_Event event;
    while (this->running) {
        this->handleEvent(event);
        this->update();
        this->render();
    }
}

void Runner::handleEvent(SDL_Event& event)
{
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_EVENT_QUIT) {
            this->running = false;
        }
    }
}

void Runner::update()
{
}

void Runner::render()
{

    SDL_RenderClear(this->renderer);
    // this->imageSprite.render(SDL_FRect { 76, 6, 88, 94 }, SDL_FRect { 0, 0, 88, 94 });
    this->imageSprite.render(
        trex.getSrc(),
        trex.getDest());
    SDL_RenderPresent(this->renderer);
}