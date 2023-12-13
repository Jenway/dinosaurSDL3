#include <SDL3/SDL_rect.h>

class TRex {
public:
    enum class Status {
        kRunning,
        kJumping,
        kDucking,
        kCrashed,
    };

    TRex()
        : mStatus { Status::kRunning }
    {
    }
    ~TRex() = default;
    void setStatus(Status status)
    {
        this->mStatus = status;
    }
    const SDL_FRect* getSrc() { return &srcRects[0]; }
    const SDL_FRect* getDest() { return &destRects[0]; }

private:
    static constexpr SDL_FRect srcRects[] = {
        { 76, 6, 88, 94 }, // steady
        { 0, 0, 44, 47 }, // kRunning
        { 88, 0, 44, 47 }, // kJumping
        { 132, 0, 59, 47 }, // kDucking
        { 220, 0, 44, 47 }, // kCrashed
    };
    static constexpr SDL_FRect destRects[] = {
        { 0, 0, 88, 94 }, // steady
        { 76, 6, 44, 47 }, // kRunning
        { 76, 6, 44, 47 }, // kJumping
        { 76, 6, 59, 47 }, // kDucking
        { 76, 6, 44, 47 }, // kCrashed
    };
    Status mStatus;
};