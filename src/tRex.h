#if !defined(__TRex_HPP__)
#define __TRex_HPP__
#include <ITexture.h>
#include <SDL3/SDL_rect.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>

class TRex {
public:
    enum class Status {
        kRunning,
        kJumping,
        kDucking,
        kCrashed,
    };
    TRex() = delete;
    TRex(ITexture& sprite)
        : sprite { sprite }
    {
    }
    ~TRex() = default;

    void update(double deltaTime, Status opt_status);

    void startJump(double speed);
    void updateJump(double deltaTime);
    void endJump();
    void setSpeedDrop();
    void setDuck(bool isDucking);
    void reset();

    const SDL_FRect* getSrc();
    const SDL_FRect* getDest() { return &this->destRect; }

private:
    static constexpr SDL_FRect srcRects[] = {
        { 76, 6, 88, 94 }, // LOGO steady1
        { 1678, 2, 88, 94 }, //  wating1
        { 44, 2, 88, 94 }, // wating2
        { 1854, 2, 88, 94 }, // running1
        { 1942, 2, 88, 94 }, // running2
        { 220, 2, 88, 94 }, // crashed1
    };
    SDL_FRect destRect = { 100, 300, 88 * 2, 94 * 2 };
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
    ITexture& sprite;
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
};
#endif // __TRex_HPP__