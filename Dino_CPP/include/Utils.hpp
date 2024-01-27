#if !defined(__UTILS_HPP__)
#define __UTILS_HPP__
#include <SDL3/SDL_rect.h>

constexpr static int FPS = 60;
static constexpr float RATE = 1.3;
constexpr static const char* const ASSETS_PATH = "assets/sprite.png";
static constexpr int SCREEN_WIDTH = 1200;
static constexpr int SCREEN_HEIGHT = 500;
static constexpr int WINDOW_WIDTH = SCREEN_WIDTH;
static constexpr int WINDOW_HEIGHT = SCREEN_HEIGHT * 1.1;
constexpr static double INITIAL_SPEED = 8 * RATE;
static constexpr float COLLISION_BOX_PERCENTAGE = 90.0f;

SDL_FRect calCollisionBox(SDL_FRect srcRect, float percentage);

#endif // !defined(__UTILS_HPP__)