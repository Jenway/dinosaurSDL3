// Interface IDrawable

#ifndef IDRAWABLE_HPP_
#define IDRAWABLE_HPP_
#include <SDL_rect.h>

class IDrawable {
public:
    [[nodiscard]] virtual SDL_FRect getSrcRect() const = 0;
    [[nodiscard]] virtual SDL_FRect getDestRect() const = 0;
};

#endif