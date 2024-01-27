
#include <Utils.hpp>
#include <algorithm>

SDL_FRect calCollisionBox(SDL_FRect srcRect, float percentage)
{
    // 确保百分比在 [0, 100] 范围内
    percentage = std::clamp(percentage, 0.0f, 100.0f);

    // 计算百分比对应的偏移量
    float offsetX = (1 - percentage / 100.0f) / 2 * srcRect.w;

    float offsetY = (1 - percentage / 100.0f) / 2 * srcRect.h;

    return SDL_FRect {
        srcRect.x + offsetX,
        srcRect.y + offsetY,
        srcRect.w - (2 * offsetX),
        srcRect.h - (2 * offsetY)
    };
}