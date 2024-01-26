#if !defined(__TRex_HPP__)
#define __TRex_HPP__
#include <IDrawable.hpp>
#include <SDL3/SDL_rect.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <array>

class TRex final : public IDrawable {
public:
    enum class Status {
        kRunning,
        kJumping,
        kDucking,
        kCrashed,
    };
    Status mStatus = Status::kRunning;

public:
    TRex() = default;
    ~TRex() = default;

    void update(float deltaTime);

    void startJump(float speed);
    void updateJump(float deltaTime);
    void endJump();

    void setSpeedDrop(bool opt);
    void setDuck(bool isDucking);

    void reset();

    [[nodiscard]] SDL_FRect getSrcRect() const override;
    [[nodiscard]] SDL_FRect getDestRect() const override;

    /*
     *Animation Frame
     */
private:
    static constexpr std::array<SDL_FRect, 10> srcRects = {
        SDL_FRect { 76, 6, 88, 94 }, // LOGO steady1
        { 1678, 2, 88, 94 }, //  wating1
        { 44, 2, 88, 94 }, // wating2
        { 1854, 2, 88, 94 }, // running1
        { 1942, 2, 88, 94 }, // running2
        { 2030, 2, 88, 94 }, // crashed1
        { 2119, 2, 88, 94 }, // crashed2
        { 2206, 36, 118, 60 }, // ducking 1
        { 2324, 36, 118, 60 } // ducking 2
    };
    // SDL_FRect destRect = { 100, 300, 88 * 2, 94 * 2 };
    static constexpr float DEST_RECT_X = 100;
    static constexpr float DEST_RECT_WIDTH = 88 * 2;
    static constexpr float DEST_RECT_HEIGHT = 94 * 2;
    SDL_FRect destRect_Ducking = { 100, 300 + 34 * 2, 118 * 2, 60 * 2 };
    static constexpr Uint32 mFrameDelay = 300 / 60;
    static constexpr int mFrameCount = 2;

    int currentFrame { 0 };
    Uint32 mTimer { 0 };

    /*
     * Animation Frame End
     */

private:
    // 加速度
    static constexpr float INITIAL_JUMP_VELOCITY = 40; // 4X
    static constexpr float SPEED_DROP_COEFFICIENT = 3;
    static constexpr float GRAVITY = 2.4;
    static constexpr float DROP_VELOCITY = 5;
    static constexpr float MAX_JUMP_HEIGHT = 0;

    static constexpr float groundYPos = 300;

    bool reachedMinHeight = true;
    bool speedDrop = false;
    float yPos { groundYPos };
    float jumpVelocity { 0 };
    int jumpCount = { 0 };

    // DEBUG

public:
    void MODIFY_STATUS(Status status) { this->mStatus = status; }
};
#endif // __TRex_HPP__