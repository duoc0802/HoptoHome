#include "Globals.h"
#include "Constants.h"
#include "GameAssets.h"
#include "Menu.h"
#include "Game.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
int main(int argc, char* argv[]) {
    if (!initSDL()) {
        std::cout << "Failed to init SDL.\n";
        return 1;
    }
    if (!loadMedia()) {
        std::cout << "Failed to load media.\n";
        return 1;
    }

    initMenuButtons();

    if (musicOn && isMenu) Mix_PlayMusic(menuMusic, -1);

    while (!quit) {
        if (isMenu) {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) quit = true;
                else if (e.type == SDL_MOUSEBUTTONDOWN) {
                    handleMenuClick(e.button.x, e.button.y);
                }
            }
            renderMenu();
            SDL_RenderPresent(gRenderer);
        } else {
            gameLoop();
            if (levelComplete) {
                currentLevel++;
                if (currentLevel > totalLevels) {
                    std::cout << "Chúc mừng! Bạn đã hoàn thành tất cả các level!\n";
                    isMenu = true;
                    currentLevel = 1;
                } else {
                    std::cout << "Level " << currentLevel - 1 << " hoàn thành! Tiếp tục Level " << currentLevel << "\n";
                }
                levelComplete = false;
            } else if (gameOver) { // Thêm điều kiện gameOver từ Game.cpp
                isMenu = true;
            }
        }
    }

    closeSDL();
    return 0;
}
