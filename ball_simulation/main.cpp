#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "ball.h"
#include <random>
#include <chrono>
#include <thread>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 960;
const int JUMP_PIXEL = 10;
int dir[4] = {0, 0, 0, 0}; // UP, DOWN, RIGHT, LEFT
int coor_x, coor_y = 0;

SDL_Window* myWindow = NULL;
SDL_Renderer* myRenderer = NULL;
Ball* myBall = NULL;
VTexture* winningImage = NULL;
VTexture* basketballBackGround = NULL;
VTexture* spaceImage = NULL;

std::mt19937_64 rng(std::chrono::system_clock::now().time_since_epoch().count());
int randnum(int l, int r) {
    l = (l + JUMP_PIXEL - 1) / JUMP_PIXEL;
    r = r / JUMP_PIXEL;
    int value = std::uniform_int_distribution<int>(l, r)(rng); 
    return value * JUMP_PIXEL;
}

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
    myBall = new Ball();
    myBall->loadTextureFromFile("ball.png", myRenderer);
    winningImage = new VTexture();
    winningImage->loadTextureFromFile("cc.png", myRenderer);
    basketballBackGround = new VTexture();
    basketballBackGround->loadTextureFromFile("bg.jpg", myRenderer);
    spaceImage = new VTexture();
    spaceImage->loadTextureFromFile("space.png", myRenderer);
    return success;
}

void winningGame() {
    SDL_SetRenderDrawColor(myRenderer, 0, 0, 0, 255);
    SDL_RenderClear(myRenderer);
    winningImage->render(0, 0, myRenderer);
    SDL_RenderPresent(myRenderer);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}

bool isContinue() {
    spaceImage->render(160, 480, myRenderer);
    SDL_RenderPresent(myRenderer);
    SDL_Event e;
    int continued = -1;
    while (continued == -1) {
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE) {
                    continued = 1;
                } else {
                    continued = 0;
                }
            }
        }
    }
    if (continued == 1) {
        coor_x = coor_y = 0;
        dir[0] = dir[1] = dir[2] = dir[3] = 0;
    }
    return continued;
}

void clear() {
    SDL_DestroyWindow(myWindow);
    myWindow = NULL;
    SDL_DestroyRenderer(myRenderer);
    myRenderer = NULL;
    delete winningImage; winningImage = NULL;
    delete basketballBackGround; basketballBackGround = NULL;
    delete spaceImage; spaceImage = NULL;
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (!init()) {
        printf("failed to init\n");
    } else {
        coor_x = 0, coor_y = WINDOW_HEIGHT - myBall->getHeight();
        bool quit = false;
        SDL_Event e;        
        // int hole_x = randnum(0, WINDOW_WIDTH - myBall->getWidth()), hole_y = randnum(0, WINDOW_HEIGHT - myBall->getHeight());
        int hole_x = 400, hole_y = 350;
        // printf("%d %d\n", hole_x, hole_y);
        while (!quit) { 
            SDL_SetRenderDrawColor(myRenderer, 0, 0, 0, 255);
			SDL_RenderClear(myRenderer);
            basketballBackGround->render(0, 0, myRenderer);
            myBall->render(coor_x, coor_y, myRenderer);
            myBall->render(hole_x, hole_y, myRenderer);
            SDL_RenderPresent(myRenderer);
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
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
                    coor_y += JUMP_PIXEL;
                } else {
                    coor_y -= JUMP_PIXEL;
                }
            } else if (dir[1]) {
                if (coor_y == WINDOW_HEIGHT - myBall->getHeight()) {
                    dir[0] = 1;
                    dir[1] = 0;
                    coor_y -= JUMP_PIXEL;
                } else {
                    coor_y += JUMP_PIXEL;
                }
            }
            if (dir[2]) {
                if (coor_x == WINDOW_WIDTH - myBall->getWidth()) {
                    dir[2] = 0;
                    dir[3] = 1;
                    coor_x -= JUMP_PIXEL;
                } else {
                    coor_x += JUMP_PIXEL;
                }
            } else if (dir[3]) {
                if (coor_x == 0) {
                    dir[2] = 1;
                    dir[3] = 0;
                    coor_x += JUMP_PIXEL;
                } else {
                    coor_x -= JUMP_PIXEL;
                }
            }
            if (coor_x == hole_x && coor_y == hole_y) {
                winningGame();
                quit = !isContinue();
            }
        }
    }
    clear();
    return 0;
}