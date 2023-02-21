#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "ball.h"
#include <windows.h>

const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 780;
int dir[4] = {0, 0, 0, 0}; // UP, DOWN, RIGHT, LEFT

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

void clear() {
    SDL_DestroyWindow(myWindow);
    myWindow = NULL;
    SDL_DestroyRenderer(myRenderer);
    myRenderer = NULL;
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (!init()) {
        printf("failed to init\n");
    } else {
        Ball* myBall = new Ball();
        myBall->loadTextureFromFile("ball.png", myRenderer);
        bool quit = false;
        SDL_Event e;
        int coor_x = 0, coor_y = WINDOW_HEIGHT - myBall->getHeight();
        while (!quit) { 
            SDL_SetRenderDrawColor(myRenderer, 0, 0, 0, 255);
			SDL_RenderClear(myRenderer);
            myBall->render(coor_x, coor_y, myRenderer);
            SDL_RenderPresent(myRenderer);
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
                if (e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.sym == SDLK_UP) {
                        dir[0] = 1;
                        dir[1] = 0;
                    }
                    if (e.key.keysym.sym == SDLK_DOWN) {
                        dir[0] = 0;
                        dir[1] = 1;
                    }
                    if (e.key.keysym.sym == SDLK_RIGHT) {
                        dir[2] = 1;
                        dir[3] = 0;
                    }
                    if (e.key.keysym.sym == SDLK_LEFT) {
                        dir[2] = 0;
                        dir[3] = 1;
                    }
                    if (e.key.keysym.sym == SDLK_SPACE) {
                        dir[0] = dir[1] = dir[2] = dir[3] = 0;
                    }
                }
            }
            if (dir[0]) {
                if (coor_y == 0) {
                    dir[0] = 0;
                    dir[1] = 1;
                    coor_y += 20;
                } else {
                    coor_y -= 20;
                }
            } else if (dir[1]) {
                if (coor_y == WINDOW_HEIGHT - myBall->getHeight()) {
                    dir[0] = 1;
                    dir[1] = 0;
                    coor_y -= 20;
                } else {
                    coor_y += 20;
                }
            }
            if (dir[2]) {
                if (coor_x == 0) {
                    dir[2] = 0;
                    dir[3] = 1;
                    coor_x += 20;
                } else {
                    coor_x -= 20;
                }
            } else if (dir[3]) {
                if (coor_x == WINDOW_WIDTH - myBall->getWidth()) {
                    dir[2] = 1;
                    dir[3] = 0;
                    coor_x -= 20;
                } else {
                    coor_x += 20;
                }
            }
            // SDL_SetRenderDrawColor(myRenderer, 0, 0, 0, 0);
            // SDL_RenderClear(myRenderer);
            // myBall->render(coor_x, coor_y, myRenderer);
            // SDL_RenderPresent(myRenderer);
            Sleep(11);
        }
    }
    clear();
    return 0;
}