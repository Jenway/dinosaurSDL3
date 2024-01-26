#if !defined(__SPRITE_HPP__)
#define __SPRITE_HPP__
// Sprite : Wrapper for original SDL_Texture
#include <IDrawable.hpp>
#include <SDL3/SDL.h>
#include <SDL3/SDL_Rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <SDL_surface.h>
#include <string_view>

class Sprite {
public:
    Sprite() = delete;
    Sprite(SDL_Renderer* gRenderer, std::string_view path);
    ~Sprite();
    void loadFromFile(std::string_view path);
    void draw(IDrawable& drawable);

private:
    SDL_Renderer* gRenderer;
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
};

#endif // __SPRITE_HPP__