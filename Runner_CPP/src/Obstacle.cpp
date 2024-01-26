#include <Data.h>
#include <Obstacle.hpp>

void Obstacle::update(float deltaTime, float speed)
{
    deltaTime = deltaTime / (1000 / 60);
    this->xPos -= speed * deltaTime;
}

SDL_FRect Obstacle::getSrcRect() const
{
    return srcRects[static_cast<int>(this->mType)];
}

SDL_FRect Obstacle::getDestRect() const
{
    return SDL_FRect { this->xPos, this->yPos, this->width, this->height };
}