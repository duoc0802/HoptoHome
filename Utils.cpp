#include "Utils.h"
#include "GameAssets.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

SDL_Texture* loadTexture(const std::string &path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cout << "IMG_Load Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_FreeSurface(surface);
    return tex;
}

void renderTexture(SDL_Texture* tex, int x, int y, int w, int h) {
    SDL_Rect dst = {x, y, w, h};
    if (w == 0 || h == 0) SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(gRenderer, tex, NULL, &dst);
}

SDL_Texture* renderText(const std::string &message, SDL_Color color) {
    SDL_Surface* surf = TTF_RenderUTF8_Blended(gFont, message.c_str(), color);
    if (!surf) return nullptr;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surf);
    SDL_FreeSurface(surf);
    return texture;
}

