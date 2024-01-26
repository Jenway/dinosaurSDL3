#if !defined(__TRex_HPP__)
#define __TRex_HPP__
#include <IDrawable.hpp>
#include <SDL3/SDL_rect.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <array>

class TRex : public IDrawable {
public:
    enum class Status {
        kRunning,
        kJumping,
        kDucking,
        kCrashed,
    };
    TRex() = default;
    ~TRex() = default;

    void update(double deltaTime);

    void startJump(double speed);
    void updateJump(double deltaTime);
    void endJump();

    void setSpeedDrop();
    void setDuck(bool isDucking);

    void reset();

    [[nodiscard]] SDL_FRect getSrcRect() const override;
    [[nodiscard]] SDL_FRect getDestRect() const override;

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
    SDL_FRect destRect = { 100, 300, 88 * 2, 94 * 2 };
    SDL_FRect destRect_Ducking = { 100, 300 + 34 * 2, 118 * 2, 60 * 2 };
    static constexpr Uint32 mFrameDelay = 300 / 60;
    static constexpr int mFrameCount = 2;

private:
    // 加速度
    static constexpr double INITIAL_JUMP_VELOCITY = -40; // 4X
    static constexpr double SPEED_DROP_COEFFICIENT = 3;
    static constexpr double GRAVITY = 0.6;
    static constexpr double DROP_VELOCITY = 20;
    static constexpr double MAX_JUMP_HEIGHT = 30;

public:
    bool jumping = false;
    bool ducking = false;
    bool speedDrop = false;

private:
    Status mStatus = Status::kRunning;
    double minJumpHeight = 120;

    double xInitialPos = 100;
    double groundYPos = 250;

    double xPos = xInitialPos;
    double yPos = groundYPos;
    double jumpVelocity = 0;

    bool midair = false;
    bool reachedMinHeight = false;

    int jumpCount = 0;

    int currentFrame = 0;
    Uint32 mTimer = 0;

    // DEBUG

public:
    void MODIFY_STATUS(Status status) { this->mStatus = status; }
};
#endif // __TRex_HPP__