#include "GameAssets.h"
#include "Utils.h"
#include "Constants.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
SDL_Window*   gWindow   = nullptr;
SDL_Renderer* gRenderer = nullptr;
TTF_Font*     gFont     = nullptr;

SDL_Texture* bgMenuTex   = nullptr;
SDL_Texture* bgGameTex   = nullptr;
SDL_Texture* rabbitTex   = nullptr;
SDL_Texture* houseTex    = nullptr;
SDL_Texture* musicOnTex  = nullptr;
SDL_Texture* musicOffTex = nullptr;
SDL_Texture* helpTex     = nullptr;
SDL_Texture* bushTex     = nullptr;
SDL_Texture* rockTex     = nullptr;

Mix_Music* menuMusic = nullptr;
Mix_Music* gameMusic = nullptr;
Mix_Music* loseMusic = nullptr;

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL init error: " << SDL_GetError() << std::endl;
        return false;
    }
    gWindow = SDL_CreateWindow("Hop to Home", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!gWindow) {
        std::cout << "CreateWindow Error: " << SDL_GetError() << std::endl;
        return false;
    }
    // Thiết lập chất lượng scale texture (nên đặt trước khi tạo renderer)
    //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!gRenderer) {
        std::cout << "CreateRenderer Error: " << SDL_GetError() << std::endl;
        return false;
    }
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "SDL_image init error: " << IMG_GetError() << std::endl;
        return false;
    }
    SDL_Surface* image = IMG_Load("back2.png");
    if (!image) {
        std::cout << "IMG_Load Error: " << IMG_GetError() << std::endl;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer init error: " << Mix_GetError() << std::endl;
        return false;
    }
    if (TTF_Init() == -1) {
        std::cout << "SDL_ttf init error: " << TTF_GetError() << std::endl;
        return false;
    }
    return true;
}

bool loadMedia() {
    gFont = TTF_OpenFont("font.ttf", 28);
    if (!gFont) return false;

    bgMenuTex   = loadTexture("backMenu.png");
    bgGameTex   = loadTexture("back2.png");
    rabbitTex   = loadTexture("rabbit1.png");
    houseTex    = loadTexture("house1.png");
    musicOnTex  = loadTexture("loa.png");
    musicOffTex = loadTexture("loa.png");
    helpTex     = loadTexture("ô.png");
    bushTex     = loadTexture("bush1.png");
    rockTex     = loadTexture("rock1.png");

    if (!bgMenuTex || !bgGameTex || !rabbitTex || !houseTex || !musicOnTex ||
        !musicOffTex || !helpTex || !bushTex || !rockTex) return false;

    menuMusic = Mix_LoadMUS("music1.wav");
    gameMusic = Mix_LoadMUS("music2.wav");
    loseMusic = Mix_LoadMUS("music3.wav");

    return (menuMusic && gameMusic && loseMusic);
}

void closeSDL() {
    SDL_DestroyTexture(bgMenuTex);
    SDL_DestroyTexture(bgGameTex);
    SDL_DestroyTexture(rabbitTex);
    SDL_DestroyTexture(houseTex);
    SDL_DestroyTexture(musicOnTex);
    SDL_DestroyTexture(musicOffTex);
    SDL_DestroyTexture(helpTex);
    SDL_DestroyTexture(bushTex);
    SDL_DestroyTexture(rockTex);

    Mix_FreeMusic(menuMusic);
    Mix_FreeMusic(gameMusic);
    Mix_FreeMusic(loseMusic);

    TTF_CloseFont(gFont);

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);

    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

