#include <Runner.hpp>
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_stdinc.h>

Runner::Runner(const SDL_Window& window, SDL_Renderer* renderer)
    : window { window }
    , renderer { renderer }
    , imageSprite(renderer, ASSETS_PATH)
    , trex { TRex {} }

{
}

void Runner::run()
{
    RUNNING_FLAG = true;
    playing = true;
    reset();
    mainLoop();
}

void Runner::reset()
{
    SDL_RenderClear(this->renderer);
    render();
}
void Runner::mainLoop()
{
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    SDL_Event event;
    while (RUNNING_FLAG) {
        frameStart = SDL_GetTicks();

        this->handleEvent(event);
        this->update(frameTime);
        // this->debugTime();
        this->render();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

void Runner::handleEvent(SDL_Event& event)
{
    auto onKeyDown = [&](SDL_Event& event) {
        if (!this->crashed && !this->paused) {
            if (!this->playing) {
                // loadSounds();
                this->playing = true;
                // this->update();
            }
            switch (event.key.keysym.sym) {
            case SDLK_SPACE:
                if (!this->trex.jumping && !this->trex.ducking) {
                    // playSound(SOUND_BUTTON_PRESS);
                    this->trex.startJump(this->currentSpeed);
                }
                break;
            case SDLK_DOWN:
                if (this->playing) {
                    if (this->trex.jumping) {
                        this->trex.setSpeedDrop();
                    } else if (!this->trex.jumping && !this->trex.ducking) {
                        this->trex.setDuck(true);
                    }
                }
                break;
            default:
                break;
            }
        }
    };
    auto onKeyUp = [&](SDL_Event& event) {
        bool isJumpKey = event.key.keysym.sym == SDLK_SPACE;

        if (this->playing && isJumpKey) {
            this->trex.endJump();
        } else if (this->trex.ducking) {
            this->trex.speedDrop = false;
            this->trex.setDuck(false);
        } else if (this->crashed) {

        } else if (this->paused && isJumpKey) {
            // this->trex.reset();
            // this->play();
        }
    };
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
        case SDL_EVENT_QUIT:
            this->RUNNING_FLAG = false;
            break;
        case SDL_EVENT_KEY_DOWN:
            onKeyDown(event);
            break;
        case SDL_EVENT_KEY_UP:
            onKeyUp(event);
            break;
        default:
            break;
        }
    }
}

void Runner::update(Uint32 frameTime)
{
    Uint32 ticks = SDL_GetTicks();
    Uint32 deltaTime = ticks - this->time;
    this->time = ticks;

    if (this->playing) {
        SDL_RenderClear(this->renderer);

        this->trex.update(deltaTime);
        // obstacles.update(deltaTime, this->currentSpeed);
    }
}
void Runner::render()
{
    imageSprite.draw(this->trex);
    SDL_RenderPresent(this->renderer);
}