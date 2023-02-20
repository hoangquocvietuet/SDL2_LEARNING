#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "ball.h"

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

SDL_Window* myWindow = NULL;
SDL_Renderer* myRenderer = NULL;

bool init() {
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("unable to init SDL, Error: %s\n", SDL_GetError());
        success = false;
    } else {
        myWindow = SDL_CreateWindow("game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
        if (myWindow == NULL) {
            printf("unable to create window, Error: %s\n", SDL_GetError());
            success = false;
        } else {
            myRenderer = SDL_CreateRenderer(myWindow, -1, SDL_RENDERER_ACCELERATED);
            if (myRenderer == NULL) {
                printf("unable to create renderer for window, Error: %s\n", SDL_GetError());
                success = false;
            } else {
                SDL_SetRenderDrawColor(myRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags) & imgFlags)) {
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
            }
        }
        
    }
    return success;
}

int main(int argc, char* argv[]) {
    if (!init()) {
        printf("failed to init\n");
    } else {
        Ball* myBall = new Ball();
        myBall->loadTextureFromFile("ball.png", myRenderer);
        bool quit = false;
        SDL_Event e;
        while (!quit) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }
            SDL_SetRenderDrawColor(myRenderer, 0, 0, 0, 255);
			SDL_RenderClear(myRenderer);
            myBall->render(50, 50, myRenderer);
            SDL_RenderPresent(myRenderer);
        }
    }
    return 0;
}