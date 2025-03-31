// GameAssets.h
#ifndef GAME_ASSETS_H
#define GAME_ASSETS_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

extern SDL_Window*   gWindow;
extern SDL_Renderer* gRenderer;
extern TTF_Font*     gFont;

extern SDL_Texture* bgMenuTex;
extern SDL_Texture* bgGameTex;
extern SDL_Texture* rabbitTex;
extern SDL_Texture* houseTex;
extern SDL_Texture* helpTex;
extern SDL_Texture* bushTex;
extern SDL_Texture* pauseTex;  // Chỉ giữ lại texture cho nút Pause
extern SDL_Texture* titleTex;
extern SDL_Texture* nextLevelTex;
extern SDL_Texture* youWinTex;
extern SDL_Texture* gameOverTex;

extern Mix_Music* menuMusic;
extern Mix_Music* gameMusic;
extern Mix_Music* loseMusic;

bool initSDL();
bool loadMedia();
void closeSDL();

#endif // GAME_ASSETS_H
