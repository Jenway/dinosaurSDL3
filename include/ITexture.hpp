// ITexture : Wrapper for original SDL_Texture
#include <SDL3/SDL.h>
#include <SDL3/SDL_Rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <SDL_surface.h>
#include <iostream>
#include <stdexcept>
#include <string_view>

class ITexture {
public:
    ITexture() = delete;
    ITexture(SDL_Renderer* gRenderer, std::string_view path)
        : gRenderer(gRenderer)
        , mTexture { nullptr }
        , mWidth { 0 }
        , mHeight { 0 }
    {
        try {
            this->loadFromFile(path);
        } catch (const std::exception& e) {
            std::clog << "ITexture::ITexture() : " << e.what() << std::endl;
        } catch (...) {
            std::clog << "ITexture::ITexture() : "
                      << "Unknown error" << std::endl;
        }
    }
    ~ITexture()
    {
        if (this->mTexture != nullptr) {
            SDL_DestroyTexture(this->mTexture);
            this->mTexture = nullptr;
        }
    }

    void loadFromFile(std::string_view path)
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

    void render(const SDL_FRect* scrRect, const SDL_FRect* destRect)
    {
        try {
            SDL_RenderTexture(gRenderer, mTexture, scrRect, destRect);

        } catch (const std::exception& e) {
            std::clog << "Runner::Runner() : " << e.what() << std::endl;
        }
    }

private:
    SDL_Renderer* gRenderer;
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
};