#include "vtexture.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string>

VTexture::VTexture() {
    mTexture = NULL;
    mHeight = mWidth = 0;
}

VTexture::~VTexture() {
    free();
}

void VTexture::free() {
    if (mTexture != NULL) {
        SDL_DestroyTexture(mTexture);
        (*this) = VTexture();
    }
}

bool VTexture::loadTextureFromFile(std::string path, SDL_Renderer* renderer) {
    free();
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    bool success = true;
    if (loadedSurface == NULL) {
        printf("Unable to load image, SDL_image Error: %s\n", IMG_GetError());
    } else {
        // default back ground's color is white
        // SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 255, 0));
        mTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (mTexture == NULL) {
            printf("Unable to create texture from image, Error: %s\n", SDL_GetError());
        } else {
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }
    return mTexture != NULL;
}

void VTexture::render(int x, int y, SDL_Renderer* renderer) {
    SDL_Rect renderRect = {x, y, mWidth, mHeight};
    SDL_RenderCopy(renderer, mTexture, NULL, &renderRect);
}

int VTexture::getHeight() {
    return mHeight;
}

int VTexture::getWidth() {
    return mWidth;
}