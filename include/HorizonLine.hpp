#include <SDL_rect.h>
#include <iostream>
#if !defined(__HORIZONLINE_HPP__)
#define __HORIZONLINE_HPP__
#include <IDrawable.hpp>
#include <Sprite.hpp>
#include <Utils.hpp>
#include <stdexcept>

class Horizon;

/*

*/

class HorizonLine final {

public:
    HorizonLine() = default;
    void update(float deltaTime, float speed)
    {
        // this->xPos -= speed * deltaTime;
        this->front += speed * deltaTime;
        if (front >= SRC_X_END) {
            this->front = SRC_X_BEGIN;
        }
        this->back = this->front + project_src_width;
    }

    // -----------------------------------------------

    void Draw(Sprite& s)
    {
        if (front >= SRC_X_BEGIN && back + project_src_width <= SRC_X_END) {
            SDL_FRect srcRect {
                front,
                112,
                project_src_width,
                project_src_height
            };
            SDL_FRect destRect {
                0,
                SCREEN_HEIGHT - height * RATE,
                SCREEN_WIDTH,
                height * RATE
            };
            s.direct_draw(srcRect, destRect);
        } else if (front < SRC_X_END && back + project_src_width >= SRC_X_END) {
            float SrcLeftLength = SRC_X_END - front;
            float SrcRightLength = project_src_width - SrcLeftLength;
            SDL_FRect srcRectLeft {
                front,
                112,
                SrcLeftLength,
                project_src_height
            };
            SDL_FRect srcRectRight {
                SRC_X_BEGIN,
                112,
                SrcRightLength,
                project_src_height
            };

            SDL_FRect destRectLeft {
                0,
                SCREEN_HEIGHT - height * RATE,
                SrcLeftLength * RATE,
                height * RATE
            };

            SDL_FRect destRectRight {
                SrcLeftLength * RATE,
                SCREEN_HEIGHT - height * RATE,
                SrcRightLength * RATE,
                height * RATE
            };

            s.direct_draw(srcRectLeft, destRectLeft);
            s.direct_draw(srcRectRight, destRectRight);

        } else {
            std::clog << "front: " << front << " back: " << back << " SRC_X_BEGIN: " << SRC_X_BEGIN << " SRC_X_END: " << SRC_X_END << "\n";
            throw std::runtime_error("HorizonLine::Draw(Sprite& s) : Invalid state");
        }
    }

private:
    static constexpr float height = 128 - 112;
    static constexpr float project_src_width = (SCREEN_WIDTH * RATE) / 2;
    static constexpr float project_src_height = (height * RATE) / 2;

    static constexpr float SRC_X_BEGIN = 2;
    // static constexpr float SRC_X_END = 2402;
    static constexpr float SRC_X_END = 2202;

private:
    float front = { SRC_X_BEGIN };
    float back = { front + project_src_width };
};

#endif