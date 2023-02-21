#include "ball.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string>

Ball::Ball() {
    ballTexture = NULL;
    ballHeight = ballWidth = 0;
}

Ball::~Ball() {
    free();
}

void Ball::free() {
    if (ballTexture != NULL) {
        SDL_DestroyTexture(ballTexture);
        (*this) = Ball();
    }
}

bool Ball::loadTextureFromFile(std::string path, SDL_Renderer* renderer) {
    free();
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    bool success = true;
    if (loadedSurface == NULL) {
        printf("Unable to load image, SDL_image Error: %s\n", IMG_GetError());
    } else {
        // default back ground's color is white
        // SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 255, 0));
        ballTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (ballTexture == NULL) {
            printf("Unable to create texture from image, Error: %s\n", SDL_GetError());
        } else {
            ballWidth = loadedSurface->w;
            ballHeight = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }
    return ballTexture != NULL;
}

void Ball::render(int x, int y, SDL_Renderer* renderer) {
    SDL_Rect renderRect = {x, y, ballWidth, ballHeight};
    SDL_RenderCopy(renderer, ballTexture, NULL, &renderRect);
}

int Ball::getHeight() {
    return ballHeight;
}

int Ball::getWidth() {
    return ballWidth;
}