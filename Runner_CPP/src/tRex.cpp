#include <cmath>
#include <iostream>
#include <tRex.hpp>

void TRex::update(double deltaTime)
{
    this->mTimer += 1;

    switch (this->mStatus) {
    case Status::kRunning:
    case Status::kDucking:
        if (this->mTimer > this->mFrameDelay) {
            this->currentFrame = (this->currentFrame + 1) % this->mFrameCount;
            this->mTimer = 0;
        }
        break;
    case Status::kJumping: {
        this->currentFrame = 0;
        this->updateJump(deltaTime);
    } break;
    case Status::kCrashed:
        this->currentFrame = 0;
        break;
    default:
        break;
    }
}

void TRex::startJump(double speed)
{
    if (!this->jumping) {
        this->mStatus = Status::kJumping;
        // Tweak the jump velocity based on the speed.
        this->jumpVelocity = INITIAL_JUMP_VELOCITY - (speed / 10);
        // std::clog << "jumpVelocity: " << this->jumpVelocity << std::endl;
        this->jumping = true;
        this->reachedMinHeight = false;
        this->speedDrop = false;
    }
}

void TRex::updateJump(double deltaTime)
{
    const double msPerFrame = 1000.0 / 60; // TODO msPerFrame[5]
    deltaTime = deltaTime / 1000;
    const double framesElapsed = deltaTime / msPerFrame;

    // Speed drop makes Trex fall faster.
    if (this->speedDrop) {
        this->yPos += round(this->jumpVelocity * this->SPEED_DROP_COEFFICIENT * framesElapsed);
    } else {
        // std::clog << "msPerFrame: " << msPerFrame << std::endl;
        // std::clog << "framesElapsed: " << framesElapsed << std::endl;
        // std::clog << "jumpVelocity: " << this->jumpVelocity << std::endl;
        this->yPos += round(this->jumpVelocity * framesElapsed);
        // this->yPos += this->jumpVelocity;
        this->destRect.y = this->yPos;
    }

    this->jumpVelocity += this->GRAVITY * framesElapsed;

    // Minimum height has been reached.
    if ((this->yPos < this->minJumpHeight) || this->speedDrop) {
        this->reachedMinHeight = true;
    }

    // Reached max height.
    if (this->yPos < this->MAX_JUMP_HEIGHT || this->speedDrop) {
        this->endJump();
    }

    // Back down at ground level. Jump completed.
    if (this->yPos > this->groundYPos) {
        this->reset();
        this->jumpCount++;

        //*if (/* * play music*/) {
        // Runner.playSound(Runner.soundFx.HIT);
    }
}

void TRex::endJump()
{
    if (this->reachedMinHeight && this->jumpVelocity < this->DROP_VELOCITY) {
        this->jumpVelocity = this->DROP_VELOCITY;
    }
}

void TRex::setSpeedDrop()
{
    this->speedDrop = true;
    this->jumpVelocity = 1;
}

void TRex::setDuck(bool isDucking)
{
    if (isDucking && this->mStatus != Status::kDucking) {
        this->ducking = true;
        this->mStatus = Status::kDucking;
    } else if (this->mStatus == Status::kDucking) {
        this->ducking = false;
        this->mStatus = Status::kRunning;
    }
}

void TRex::reset()
{
    this->xPos = this->xInitialPos;
    this->yPos = this->groundYPos;
    this->jumpVelocity = 0;
    this->jumping = false;
    this->ducking = false;
    // this->update(0, Trex.status.RUNNING);
    this->mStatus = Status::kRunning;
    this->midair = false;
    this->speedDrop = false;
    this->jumpCount = 0;
}

SDL_FRect TRex::getSrcRect() const
{
    switch (this->mStatus) {
    case Status::kRunning:
        return srcRects[this->currentFrame + 3];
    case Status::kJumping:
        return srcRects[1];
    case Status::kDucking:
        return srcRects[this->currentFrame + 7];
    case Status::kCrashed:
        return srcRects[4];
    default:
        throw std::runtime_error("TRex::getSrcRect() error");
    }
}
SDL_FRect TRex::getDestRect() const
{

    switch (this->mStatus) {
    case Status::kRunning:
    case Status::kJumping:
    case Status::kCrashed:
        return this->destRect;
    case Status::kDucking:
        return this->destRect_Ducking;
    default:
        throw std::runtime_error("TRex::getSrcRect() error");
    }
}
