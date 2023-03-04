#include <stdio.h>
#include "LGame.h"
/*
    window to display on
*/
SDL_Window* gWindow = NULL;

/*
    renderer of the window
*/
SDL_Renderer* gRenderer = NULL;

// global used font
TTF_Font* gFont = NULL;

bool init() {
    bool success = true;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    } else {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf( "Warning: Linear texture filtering not enabled!" );
        }
        gWindow = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        } else {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL) {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            } else {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags) & imgFlags)) {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );					
                    success = false;
				}
                if( TTF_Init() == -1 ) {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }
                gFont = TTF_OpenFont("lazy.ttf", 35);
                if(gFont == NULL) {
                    printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }
            }
        }
    }
    return success;
}

void close() {
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;
    TTF_CloseFont(gFont);
    gFont = NULL;
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}

int main(int argc, char* argv[]) {
    if (!init()) {
        printf("Failed to initialize\n");
    } else {
        LGame game(gRenderer, gFont);
        game.playGame();
    }

    close();
    return 0;
}