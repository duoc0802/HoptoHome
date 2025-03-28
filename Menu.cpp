#include "Menu.h"
#include "GameAssets.h"
#include "Utils.h"
#include "Constants.h"
#include "Globals.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
std::vector<Button> menuButtons;

bool showHelp     = false;

void initMenuButtons() {
    menuButtons.clear();
    menuButtons.push_back({{220, 200, 200, 60}, BTN_START});
    menuButtons.push_back({{220, 300, 200, 60}, BTN_MUSIC});
    menuButtons.push_back({{220, 400, 200, 60}, BTN_HELP});
}

void handleMenuClick(int x, int y) {
    for (auto &btn : menuButtons) {
        if (x >= btn.box.x && x <= btn.box.x + btn.box.w &&
            y >= btn.box.y && y <= btn.box.y + btn.box.h) {
            switch (btn.type) {
                case BTN_START:
                    isMenu = false;
                    Mix_HaltMusic();
                    if (musicOn) Mix_PlayMusic(gameMusic, -1);
                    break;
                case BTN_MUSIC:
                    musicOn = !musicOn;
                    if (musicOn) {
                        if (musicPaused) {
                            Mix_ResumeMusic();
                            musicPaused = false;
                        } else {
                            Mix_PlayMusic(isMenu ? menuMusic : gameMusic, -1);
                        }
                    } else {
                        Mix_PauseMusic();
                        musicPaused = true;
                    }
                    break;
                case BTN_HELP:
                    showHelp = !showHelp;
                    break;
            }
        }
    }
}

void renderMenu() {
    renderTexture(bgMenuTex, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    for (auto &btn : menuButtons) {
        SDL_SetRenderDrawColor(gRenderer, 128, 128, 128, 255);
        SDL_RenderFillRect(gRenderer, &btn.box);
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(gRenderer, &btn.box);

        std::string text;
        if (btn.type == BTN_START) text = "START";
        else if (btn.type == BTN_MUSIC) text = (musicOn ? "MUSIC: ON" : "MUSIC: OFF");
        else if (btn.type == BTN_HELP) text = "GUIDE";

        SDL_Color white = {255, 255, 255, 255};
        SDL_Texture* textTex = renderText(text, white);
        if (textTex) {
            int tw, th;
            SDL_QueryTexture(textTex, NULL, NULL, &tw, &th);
            renderTexture(textTex, btn.box.x + (btn.box.w - tw) / 2, btn.box.y + (btn.box.h - th) / 2);
            SDL_DestroyTexture(textTex);
        }
    }

    if (showHelp) {
        renderTexture(helpTex, 50, 50, SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100);
    }
}

