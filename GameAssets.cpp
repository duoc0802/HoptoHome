// GameAssets.cpp
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
SDL_Texture* bgOutsideTex = nullptr;  // Texture cho nền bên ngoài khu vực chơi game
SDL_Texture* rabbitTex   = nullptr;
SDL_Texture* houseTex    = nullptr;
SDL_Texture* musicOnTex  = nullptr;
SDL_Texture* musicOffTex = nullptr;
SDL_Texture* helpTex     = nullptr;
SDL_Texture* bushTex     = nullptr;
SDL_Texture* rockTex     = nullptr;
SDL_Texture* pauseTex    = nullptr;  // Chỉ giữ lại texture cho nút Pause
SDL_Texture* titleTex = nullptr;
SDL_Texture* nextLevelTex = nullptr;
SDL_Texture* youWinTex = nullptr;
SDL_Texture* gameOverTex = nullptr;

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
    SDL_Surface* image = IMG_Load("backgame.jpg");
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
    gFont = TTF_OpenFont("font2.ttf", 22);
    if (!gFont) return false;

    bgMenuTex    = loadTexture("back5.png");
    bgGameTex    = loadTexture("backgame.jpg");
    bgOutsideTex = loadTexture("back4.png");  // Tải hình ảnh mới cho nền bên ngoài
    rabbitTex    = loadTexture("rabbit2.png");
    houseTex     = loadTexture("house1.png");
    helpTex      = loadTexture("guide.png");
    bushTex      = loadTexture("bush3.png");
    pauseTex     = loadTexture("pause.png");  // Chỉ giữ lại texture cho nút Pause
    titleTex     = loadTexture("title1.png");
    nextLevelTex = loadTexture("nextlevel1.png");
    youWinTex    = loadTexture("youwin1.png");
    gameOverTex  = loadTexture("gameover1.png");

    if (!bgMenuTex || !bgGameTex || !bgOutsideTex || !rabbitTex || !houseTex || !helpTex || !bushTex || !pauseTex || !titleTex || !nextLevelTex || !youWinTex || !gameOverTex) return false;

    menuMusic = Mix_LoadMUS("music1.wav");
    gameMusic = Mix_LoadMUS("music2.wav");
    loseMusic = Mix_LoadMUS("music3.wav");

    return (menuMusic && gameMusic && loseMusic);
}

void closeSDL() {
    SDL_DestroyTexture(bgMenuTex);
    SDL_DestroyTexture(bgGameTex);
    SDL_DestroyTexture(bgOutsideTex);  // Hủy texture nền bên ngoài
    SDL_DestroyTexture(rabbitTex);
    SDL_DestroyTexture(houseTex);;
    SDL_DestroyTexture(helpTex);
    SDL_DestroyTexture(bushTex);
    SDL_DestroyTexture(pauseTex);  // Hủy texture Pause
    SDL_DestroyTexture(titleTex);
    SDL_DestroyTexture(nextLevelTex);
    SDL_DestroyTexture(youWinTex);
    SDL_DestroyTexture(gameOverTex);

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
