#if !defined(__HORIZONLINE_HPP__)
#define __HORIZONLINE_HPP__
#include <Data.h>
#include <IDrawable.hpp>

class HorizonLine final : public IDrawable {

public:
    HorizonLine() = default;
    void update(float deltaTime, float speed) override
    {
        deltaTime = deltaTime / (1000 / 60);
        this->xSrcPos += (this->speed * deltaTime) / 2;
        if (this->xSrcPos >= this->xSrcLimit) {
            this->xSrcPos = this->xSrcBegin;
        }
    }
    [[nodiscard]] SDL_FRect getSrcRect() const override
    {
        return SDL_FRect { xSrcPos, 112, SCREEN_WIDTH / RATE, 128 - 112 };
    }
    [[nodiscard]] SDL_FRect getDestRect() const override
    {
        return SDL_FRect { 0, SCREEN_HEIGHT - height * RATE, SCREEN_WIDTH, height * RATE };
    }

private:
    float height = 128 - 112;
    float speed = 40;
    float xSrcBegin = 2;
    float xSrcPos { 2 };
    float xSrcLimit = 2402;
};

#endif