#include <cmath>
#include <iostream>
#include <tRex.hpp>

void TRex::update(float deltaTime)
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

void TRex::startJump(float speed)
{
    if (mStatus != Status::kJumping) {
        this->mStatus = Status::kJumping;
        this->jumpVelocity = -INITIAL_JUMP_VELOCITY;
        this->speedDrop = false;
    }
}

void TRex::endJump()
{
    if (this->reachedMinHeight && this->jumpVelocity < this->DROP_VELOCITY) {
        this->jumpVelocity = this->DROP_VELOCITY;
    }
}

void TRex::updateJump(float deltaTime)
{
    const float framesElapsed = deltaTime / 20;
    // std::clog << "framesElapsed: " << framesElapsed << std::endl;
    // TODO speed drop
    if (this->speedDrop) {
        this->yPos += round(this->jumpVelocity * framesElapsed * SPEED_DROP_COEFFICIENT);
    } else {
        this->yPos += round(this->jumpVelocity * framesElapsed);
    }
    // std::clog << "yPos: " << this->yPos << std::endl;
    this->jumpVelocity += this->GRAVITY * framesElapsed;
    // std::clog << "jumpVelocity: " << this->jumpVelocity << std::endl;

    //   TODO MINUIM HEIGHT

    // Reached max height.
    if (this->yPos < this->MAX_JUMP_HEIGHT || this->speedDrop) {
        this->endJump();
    }

    // Back down at ground level. Jump completed.
    if (this->yPos >= this->groundYPos) {
        this->reset();
        // TODO JUMP COUNT
    }
}

void TRex::setSpeedDrop(bool opt)
{
    if (opt) {
        this->speedDrop = true;
    } else {
        this->speedDrop = false;
    }
}

void TRex::setDuck(bool isDucking)
{
    if (isDucking && this->mStatus != Status::kDucking) {
        this->mStatus = Status::kDucking;
    } else if (this->mStatus == Status::kDucking) {
        this->mStatus = Status::kRunning;
    }
}

void TRex::reset()
{
    this->yPos = this->groundYPos;
    this->jumpVelocity = 0;
    this->mStatus = Status::kRunning;
    this->speedDrop = false;
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
        return { DEST_RECT_X, this->yPos, DEST_RECT_WIDTH, DEST_RECT_HEIGHT };
    case Status::kDucking:
        return this->destRect_Ducking;
    default:
        throw std::runtime_error("TRex::getSrcRect() error");
    }
}
