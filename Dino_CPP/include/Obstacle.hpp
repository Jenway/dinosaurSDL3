
#include <SDL_rect.h>
#if !defined(__OBSTACLE_HPP__)
#define __OBSTACLE_HPP__
#include <IDrawable.hpp>
#include <SDL_stdinc.h>
#include <Utils.hpp>
#include <array>
#include <iostream>

class Obstacle final : public IDrawable {
public:
    enum class Type {
        kSmallCactus1,
        kSmallCactus2,
        kSmallCactus3,
        kLargeCactus1,
        kLargeCactus2,
        kLargeCactus3,
    };
    Type mType = Type::kSmallCactus1;

public:
    Obstacle() = delete;
    Obstacle(Type type)
        : mType { type }
        , yPos { SCREEN_HEIGHT - srcRects[static_cast<int>(this->mType)].h * RATE }
        , width { srcRects[static_cast<int>(this->mType)].w * RATE }
        , height { srcRects[static_cast<int>(this->mType)].h * RATE }
    {
        xPos = SCREEN_WIDTH + 2 * width;
    }
    ~Obstacle() = default;
    bool remove { false };
    [[nodiscard]] bool isVisible() const
    {
        return (this->xPos + this->width) > 0;
    }

    void update(float deltaTime, float speed) override;
    [[nodiscard]] SDL_FRect getSrcRect() const override;
    [[nodiscard]] SDL_FRect getDestRect() const override;

    [[nodiscard]] SDL_FRect getCollisionBox() const
    {
        return calCollisionBox(this->getDestRect(), 98.0f);
    }

private:
    /*
    static constexpr Uint32 Small_cactus_x = 446;
        //  446 480 514 548 582 616 650
        static constexpr Uint32 Small_cactus_y = 2;
        static constexpr Uint32 Small_cactus_width = 34;
        static constexpr Uint32 Small_cactus_height = 70;
        static constexpr Uint32 Small_cactus_count = 6;

        static constexpr Uint32 Large_cactus_x = 652;
        //  702 750 752 802 850 952
        static constexpr Uint32 Large_cactus_y = 2;
        static constexpr Uint32 Large_cactus_width = 34;
        static constexpr Uint32 Large_cactus_height = 100;
    */

    static constexpr std::array<SDL_FRect, 6> srcRects = {
        SDL_FRect { 652, 2, 750 - 652, 100 }, // Large_cactus_2
        { 752, 2, 802 - 752, 100 }, // Large_cactus_1
        { 802, 2, 952 - 802, 100 }, // Large_cacltus_Mix
        { 446, 2, 34, 70 }, // Small_cactus_1
        { 480, 2, 34 * 2, 70 }, // Small_cactus_2
        { 548, 2, 34 * 3, 70 }, // Small_cactus_3
    };

    int currentFrame { 0 };
    Uint32 mTimer { 0 };

    float xPos { 0 };
    float yPos;
    float width;
    float height;
};

#endif