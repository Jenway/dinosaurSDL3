#include <Sprite.hpp>
#include <iostream>
#include <stdexcept>

Sprite::Sprite(SDL_Renderer* gRenderer, std::string_view path)
    : gRenderer(gRenderer)
    , mTexture { nullptr }
    , mWidth { 0 }
    , mHeight { 0 }
{
    try {
        this->loadFromFile(path);
    } catch (const std::exception& e) {
        std::clog << "Sprite::Sprite() : " << e.what() << std::endl;
    } catch (...) {
        std::clog << "Sprite::Sprite() : "
                  << "Unknown error" << std::endl;
    }
}

Sprite::~Sprite()
{
    if (this->mTexture != nullptr) {
        SDL_DestroyTexture(this->mTexture);
        this->mTexture = nullptr;
    }
}

inline void Sprite::loadFromFile(std::string_view path)
{

    if (this->mTexture != nullptr) {
        SDL_DestroyTexture(this->mTexture);
        this->mTexture = nullptr;
    }

    SDL_Texture* newTexture = IMG_LoadTexture(this->gRenderer, path.data());
    if (newTexture == nullptr) {
        throw std::runtime_error("Failed to load texture from " + std::string(path));
    }
    SDL_QueryTexture(newTexture, nullptr, nullptr, &this->mWidth, &this->mHeight);

    SDL_Surface* loadedSurface = IMG_Load(path.data());
    if (loadedSurface == nullptr) {
        throw std::runtime_error("Failed to load surface from " + std::string(path));
    }
    SDL_SetSurfaceBlendMode(loadedSurface, SDL_BLENDMODE_BLEND);
    SDL_SetSurfaceColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));
    newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

    SDL_DestroySurface(loadedSurface);

    this->mTexture = newTexture;
}

void Sprite::draw(IDrawable& drawable)
{
    try {
        SDL_FRect srcRect { drawable.getSrcRect() };
        SDL_FRect destRect { drawable.getDestRect() };
        SDL_RenderTexture(gRenderer, mTexture, &srcRect, &destRect);

    } catch (const std::exception& e) {
        std::clog << "Sprite::draw() : " << e.what() << std::endl;
    }
}