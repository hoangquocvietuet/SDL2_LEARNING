#ifndef BALL_H
#define BALL_H
#include <SDL.h>
#include <string>

class Ball {
    public:
        Ball();
        ~Ball();
        bool loadTextureFromFile(std::string path, SDL_Renderer* renderer);
        void free();
        void render(int x, int y, SDL_Renderer* renderer);
        // int getWidth();
        // int getHeight();
    private:
        SDL_Texture* ballTexture;
        int ballWidth;
        int ballHeight;
};

#endif