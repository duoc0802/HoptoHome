#include "Level.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

LevelManager::LevelManager() {
    // Khởi tạo level mặc định
    currentLevel.rabbitPos = {0, 0};
    currentLevel.housePos = {4, 4};
    currentLevel.obstacles.clear();
    currentLevel.bridges.clear();
}

void LevelManager::loadLevel(int levelNumber) {
    switch (levelNumber) {
        case 1:
            currentLevel.rabbitPos = {0, 0};
            currentLevel.housePos = {4, 4};
            //currentLevel.obstacles = {{1, 1}, {2, 2}, {3, 3}};
            //currentLevel.bridges = {{2, 1}};
            break;
        case 2:
            currentLevel.rabbitPos = {0, 4};
            currentLevel.housePos = {3, 2};
            currentLevel.obstacles = {{3, 1}, {3, 4}, {4, 4}};
            currentLevel.bridges = {{1, 3}, {3, 2}};
            break;
        default:
            // Level mặc định nếu levelNumber không hợp lệ
            currentLevel.rabbitPos = {0, 0};
            currentLevel.housePos = {4, 4};
            currentLevel.obstacles.clear();
            currentLevel.bridges.clear();
            break;
    }
}

const Level& LevelManager::getCurrentLevel() const {
    return currentLevel;
}
