// The idea of this code mainly comes from
// Lazy Foo' Productions (https://lazyfoo.net/tutorials/SDL/index.php)

#if !defined(_LTEXTURE_H_)
#define _LTEXTURE_H_

#include <SDL3/SDL.h>
#include <stdbool.h>

typedef struct ltexture {
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;

    // methods
    bool (*loadFromFile)(struct ltexture* this, SDL_Renderer* gRenderer, char* path);
    void (*render)(struct ltexture* this, SDL_Renderer* gRenderer, const SDL_FRect* scrRect, const SDL_FRect* destRect);
} LTexture;

LTexture* LTexture_constructor();
void LTexture_destructor(LTexture* this);
bool LTexture_loadFromFile(LTexture* this, SDL_Renderer* gRenderer, char* path);
void LTexture_free(LTexture* this);
void LTexture_render(LTexture* this, SDL_Renderer* gRenderer, const SDL_FRect* scrRect, const SDL_FRect* destRect);

#endif // _LTEXTURE_H_