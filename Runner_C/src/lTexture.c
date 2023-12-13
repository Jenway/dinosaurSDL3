// Path: src\lTexture.c
#include <SDL3_image/SDL_image.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <lTexture.h>
#include <stdio.h>
#include <stdlib.h>

LTexture* LTexture_constructor()
{
    LTexture* this = malloc(sizeof(LTexture));
    this->mTexture = NULL;
    this->mWidth = 0;
    this->mHeight = 0;
    // methods
    this->loadFromFile = LTexture_loadFromFile;
    this->render = LTexture_render;
    this->free = LTexture_destructor;
    return this;
}
void LTexture_destructor(LTexture* this)
{
    LTexture_free(this);
    free(this);
    this = NULL;
}

bool LTexture_loadFromFile(LTexture* this, SDL_Renderer* gRenderer, char* path)
{
    LTexture_free(this);
    SDL_Texture* newTexture = NULL;

    newTexture = IMG_LoadTexture(gRenderer, path);
    SDL_QueryTexture(newTexture, NULL, NULL, &this->mWidth, &this->mHeight);

    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
    } else {
        // SDL_SetSurfaceColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
        // ffffff
        SDL_SetSurfaceColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

        SDL_DestroySurface(loadedSurface);
    }
    this->mTexture = newTexture;
    return this->mTexture != NULL;
}

void LTexture_free(LTexture* this)
{
    if (this->mTexture != NULL) {
        SDL_DestroyTexture(this->mTexture);
        this->mTexture = NULL;
        this->mWidth = 0;
        this->mHeight = 0;
    }
}

void LTexture_render(LTexture* this, SDL_Renderer* gRenderer, const SDL_FRect* srcRect, const SDL_FRect* destRect)
{

    SDL_RenderTexture(gRenderer, this->mTexture, srcRect, destRect);
}
