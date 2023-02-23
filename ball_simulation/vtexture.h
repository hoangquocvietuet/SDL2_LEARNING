#ifndef VTEXTURE_H
#define VTEXTURE_H
#include <SDL.h>
#include <string>
class VTexture {
    public:
        VTexture();
        ~VTexture();
        bool loadTextureFromFile(std::string path, SDL_Renderer* renderer);
        void free();
        void render(int x, int y, SDL_Renderer* renderer);
        int getWidth();
        int getHeight();
    private:
        SDL_Texture* mTexture;
        int mWidth;
        int mHeight;
};

#endif