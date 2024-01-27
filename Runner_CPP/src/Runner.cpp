#include <Runner.hpp>
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_stdinc.h>
#include <iostream>

Runner::Runner(const SDL_Window& window, SDL_Renderer* renderer)
    : window { window }
    , renderer { renderer }
    , imageSprite(renderer, ASSETS_PATH)
    , trex { TRex {} }
    , horizon { Horizon {} }

{
}

void Runner::run()
{
    RUNNING_FLAG = true;
    horizon.clear();
    SDL_RenderClear(this->renderer);
    render();
    crashed = false;
    playing = false;
    paused = true;
    mainLoop();
}

void Runner::reset()
{
    trex.reset();
    horizon.clear();
    SDL_RenderClear(this->renderer);
    render();
    crashed = false;
    paused = false;
    playing = false;
}

void Runner::play()
{
    if (!crashed) {
        paused = false;
        playing = true;
    } else {
        reset();
        playing = false;
    }
}

void Runner::mainLoop()
{
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    SDL_Event event;
    while (RUNNING_FLAG) {
        frameStart = SDL_GetTicks();
        // std::clog << "frameStart: " << frameStart << std::endl;
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
    // TODO extern DECLSPEC SDL_Window * SDLCALL SDL_GetKeyboardFocus(void);

    const Uint8* keyState = SDL_GetKeyboardState(nullptr);

    if (keyState[SDL_SCANCODE_ESCAPE]) {
        this->RUNNING_FLAG = false;
    }

    if (!this->crashed && !this->paused) {
        if (!this->playing) {
            // loadSounds();
            this->playing = true;

            // this->update();
        }
        if (keyState[SDL_SCANCODE_SPACE] || keyState[SDL_SCANCODE_UP]) {
            if (trex.mStatus != TRex::Status::kJumping && trex.mStatus != TRex::Status::kDucking) {
                // playSound(SOUND_BUTTON_PRESS);
                this->trex.startJump(this->currentSpeed);
            }
        } else if (keyState[SDL_SCANCODE_DOWN]) {
            if (this->playing) {
                if (this->trex.mStatus == TRex::Status::kJumping) {
                    this->trex.setSpeedDrop(true);
                } else if (trex.mStatus == TRex::Status::kRunning) {
                    this->trex.setDuck(true);
                }
            }
        }
    } else if (this->crashed) {
        if (keyState[SDL_SCANCODE_SPACE] || keyState[SDL_SCANCODE_UP]) {
            this->reset();
        }
    } else if (this->paused) {
        if (keyState[SDL_SCANCODE_SPACE] || keyState[SDL_SCANCODE_UP]) {
            this->play();
        }
    }

    auto onKeyUp = [&](SDL_Event& event) {
        bool isJumpKey = event.key.keysym.sym == SDLK_SPACE;

        if (this->trex.mStatus == TRex::Status::kRunning && isJumpKey) {
            this->trex.endJump();
        } else if (trex.mStatus == TRex::Status::kDucking) {
            this->trex.setSpeedDrop(false);
            this->trex.setDuck(false);
        } else if (this->crashed) {

        } else if (this->paused && isJumpKey) {
            // this->trex.reset();
            // this->play();
        }
    };

    if (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
        case SDL_EVENT_QUIT:
            this->RUNNING_FLAG = false;
            break;
        case SDL_EVENT_KEY_UP:
            onKeyUp(event);
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

        this->trex.update(deltaTime, this->currentSpeed);
        this->horizon.update(deltaTime, this->currentSpeed);
        if (this->horizon.checkCollision(trex.getCollisionBox())) {
            this->crashed = true;
            this->playing = false;
            trex.mStatus = TRex::Status::kCrashed;
            // playSound(SOUND_HIT);
            // vibrate(200);
            // this->stop();
        }
    }
}
void Runner::render()
{
    // auto DrawRect = [&](const SDL_FRect& rect) {
    //     SDL_SetRenderDrawColor(this->renderer, 0xFF, 0x35, 0x67, 0xFF);
    //     SDL_RenderRect(this->renderer, &rect);
    //     SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    // };
    // DrawRect(this->trex.getCollisionBox());
    // std::clog << "trex.getCollisionBox(): " << this->trex.getCollisionBox().x << ", " << this->trex.getCollisionBox().y << ", " << this->trex.getCollisionBox().w << ", " << this->trex.getCollisionBox().h << std::endl;
    // for (auto& o : this->horizon.obstacles) {
    //     DrawRect(o.getCollisionBox());
    // }

    horizon.draw(imageSprite);
    imageSprite.draw(this->trex);

    SDL_RenderPresent(this->renderer);
}