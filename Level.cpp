// Level.cpp
#include "Level.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

LevelManager::LevelManager() {
    // Khởi tạo level mặc định
    currentLevel.rabbitPos = {3, 2};
    currentLevel.housePos = {4, 0};
    currentLevel.obstacles.clear();
    currentLevel.rows = 7;  // Mặc định 5 hàng
    currentLevel.cols = 4;  // Mặc định 7 cột
}

void LevelManager::loadLevel(int levelNumber) {
    switch (levelNumber) {
        case 1:
            currentLevel.rabbitPos = {1, 5};
            currentLevel.housePos = {2, 2};
            currentLevel.obstacles = {{1, 1}, {1, 2}, {1, 3}, {2,1}, {3,2}, {2,4}, {0,5}, {2,5}, {4,4}, {5,4}, {6,2}};
            currentLevel.rows = 7;
            currentLevel.cols = 6;
            break;
        case 2:
            currentLevel.rabbitPos = {2, 4};
            currentLevel.housePos = {4, 0};
            currentLevel.obstacles = {{0, 4}, {2, 2}, {2, 3}, {3,0}};
            currentLevel.rows = 5;
            currentLevel.cols = 5;
            break;
        case 3:
            currentLevel.rabbitPos = {0, 1};
            currentLevel.housePos = {4, 0};
            currentLevel.obstacles = {{1, 1}, {2, 2}, {3, 0}, {4,1}, {4, 2}, {4, 3}};
            currentLevel.rows = 6;
            currentLevel.cols = 5;
            break;
        case 4:
            currentLevel.rabbitPos = {0, 3};
            currentLevel.housePos = {4, 0};
            currentLevel.obstacles = {{1, 4}, {1, 1}, {2, 2}, {3,0}, {3,3}, {4,1}, {4,3}, {5,3}};
            currentLevel.rows = 6;
            currentLevel.cols = 6;
            break;
        case 5:
            currentLevel.rabbitPos = {1, 0};
            currentLevel.housePos = {1, 3};
            currentLevel.obstacles = {{0, 2}, {0, 3}, {2, 3}, {5,3}};
            currentLevel.rows = 6;
            currentLevel.cols = 4;
            break;
        case 6:
            currentLevel.rabbitPos = {0, 1};
            currentLevel.housePos = {5, 0};
            currentLevel.obstacles = {{4, 2}, {4, 3}, {5, 1}, {5,2}, {5,3}};
            currentLevel.rows = 6;
            currentLevel.cols = 4;
            break;
        case 7:
            currentLevel.rabbitPos = {7, 4};
            currentLevel.housePos = {2, 3};
            currentLevel.obstacles = {{1, 1}, {1, 2}, {2, 2}, {3,0}, {4,0}, {2,4}, {3,3}, {2,5}, {4,3}, {7,0}, {6,2}, {5,2}, {6,4}};
            currentLevel.rows = 8;
            currentLevel.cols = 6;
            break;
        case 8:
            currentLevel.rabbitPos = {4, 2};
            currentLevel.housePos = {8, 5};
            currentLevel.obstacles = {{2, 0}, {2, 1}, {2, 2}, {3,2}, {7,1}, {0,5}, {1,5}, {2,5}, {8,1}, {8,6}};
            currentLevel.rows = 10;
            currentLevel.cols = 8;
            break;
        case 9:
            currentLevel.rabbitPos = {3, 5};
            currentLevel.housePos = {6, 5};
            currentLevel.obstacles = {{0, 0}, {1, 2}, {0, 5}, {1,5}, {3,2}, {6,0}, {6,1}, {6,4}};
            currentLevel.rows = 7;
            currentLevel.cols = 6;
            break;
        case 10:
            currentLevel.rabbitPos = {1, 2};
            currentLevel.housePos = {5, 3};
            currentLevel.obstacles = {{1, 1}, {2, 2}, {0, 3}, {1,3}, {1,5}, {2,5}, {4,1}, {4,3}, {4,6}, {5,4}, {6,0}, {6,3}, {8,4}};
            currentLevel.rows = 9;
            currentLevel.cols = 7;
            break;
        case 11:
            currentLevel.rabbitPos = {4, 0};
            currentLevel.housePos = {0, 3};
            currentLevel.obstacles = {{0, 2}, {0, 4}, {2, 3}, {5,0}, {6,0}, {5,4}, {6,4}};
            currentLevel.rows = 7;
            currentLevel.cols = 5;
            break;
        case 12:
            currentLevel.rabbitPos = {2, 6};
            currentLevel.housePos = {4, 0};
            currentLevel.obstacles = {{2, 2}, {2, 3}, {3, 4}, {3,5}, {4,1}, {5,0}, {5,3}, {5,5}, {5,6}, {6,2}, {6,3}};
            currentLevel.rows = 8;
            currentLevel.cols = 7;
            break;
        case 13:
            currentLevel.rabbitPos = {1, 5};
            currentLevel.housePos = {5, 0};
            currentLevel.obstacles = {{2, 0}, {2, 5}, {3, 0}, {3,1}, {4,0}, {4,1}};
            currentLevel.rows = 6;
            currentLevel.cols = 6;
            break;
        case 14:
            currentLevel.rabbitPos = {8, 0};
            currentLevel.housePos = {8, 2};
            currentLevel.obstacles = {{0, 2}, {1, 2}, {2, 3}, {1,5}, {2,5}, {3,1}, {4,3}, {5,3}, {4,6}, {6,0}, {6,5}, {7,5}, {8,3}, {8,1}};
            currentLevel.rows = 9;
            currentLevel.cols = 7;
            break;
        case 15:
            currentLevel.rabbitPos = {6, 2};
            currentLevel.housePos = {6, 0};
            currentLevel.obstacles = {{0, 4}, {2, 1}, {4, 4}, {6,1}};
            currentLevel.rows = 7;
            currentLevel.cols = 5;
            break;
        default:
            // Level mặc định nếu levelNumber không hợp lệ
            currentLevel.rabbitPos = {3, 2};
            currentLevel.housePos = {4, 0};
            currentLevel.obstacles.clear();
            currentLevel.rows = 7;
            currentLevel.cols = 4;
            break;
    }
}

const Level& LevelManager::getCurrentLevel() const {
    return currentLevel;
}
