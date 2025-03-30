// Game.cpp
#include "Game.h"
#include "GameAssets.h"
#include "Utils.h"
#include "Constants.h"
#include "Level.h"
#include "Globals.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>

// Khai báo các biến toàn cục
LevelManager levelManager;
int currentLevel = 1;
bool levelComplete = false;  // Định nghĩa biến
bool gameOver = false;      // Định nghĩa biến
int rabbitRow, rabbitCol;
int houseRow, houseCol;
int gameAreaX, gameAreaY;
int score = 0;
int TILE_SIZE = 32;
std::vector<std::pair<int, int>> obstacles;
std::vector<std::pair<int, int>> bridges;
std::vector<std::pair<int, int>> path;
Uint32 startTime = 0;
int timeLeft = GAME_TIME;
int rows, cols;
int gameAreaWidth, gameAreaHeight;
bool isPaused = false;
Uint32 pauseStartTime = 0;
Uint32 totalPausedTime = 0;

// Biến để lưu kích thước và vị trí của nút Pause
int pauseButtonX = 0;
int pauseButtonY = 0;
int pauseButtonWidth = 60;  // Tăng kích thước nút
int pauseButtonHeight = 60;

bool isObstacle(int row, int col) {
    for (auto &obs : obstacles) {
        if (obs.first == row && obs.second == col) return true;
    }
    return false;
}

bool checkWin() {
    std::set<std::pair<int, int>> uniquePath(path.begin(), path.end());
    size_t totalCells = rows * cols - obstacles.size();
    return uniquePath.size() == totalCells &&
           path.front() == std::make_pair(rabbitRow, rabbitCol) &&
           path.back() == std::make_pair(houseRow, houseCol);
}

void renderGrid() {
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    for (int r = 0; r <= rows; r++) {
        SDL_RenderDrawLine(gRenderer, 0, r * TILE_SIZE, gameAreaWidth, r * TILE_SIZE);
    }
    for (int c = 0; c <= cols; c++) {
        SDL_RenderDrawLine(gRenderer, c * TILE_SIZE, 0, c * TILE_SIZE, gameAreaHeight);
    }
}

void renderGame() {
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);

    SDL_RenderSetViewport(gRenderer, NULL);
    renderTexture(bgOutsideTex, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Rect gameArea = {gameAreaX, gameAreaY, gameAreaWidth, gameAreaHeight};
    SDL_RenderSetViewport(gRenderer, &gameArea);

    renderTexture(bgGameTex, 0, 0, gameAreaWidth, gameAreaHeight);

    renderGrid();

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (isObstacle(r, c)) {
                renderTexture(bushTex, c * TILE_SIZE, r * TILE_SIZE, TILE_SIZE, TILE_SIZE);
            }
        }
    }

    SDL_SetRenderDrawColor(gRenderer, 250, 170, 141, 255);
    int thickness = 20;

    if (path.size() > 1) {
        for (size_t i = 0; i + 1 < path.size(); i++) {
            int r1 = path[i].first, c1 = path[i].second;
            int r2 = path[i + 1].first, c2 = path[i + 1].second;

            int x1 = c1 * TILE_SIZE + TILE_SIZE / 2;
            int y1 = r1 * TILE_SIZE + TILE_SIZE / 2;
            int x2 = c2 * TILE_SIZE + TILE_SIZE / 2;
            int y2 = r2 * TILE_SIZE + TILE_SIZE / 2;

            if (x1 == x2) {
                int topY = std::min(y1, y2);
                int height = abs(y2 - y1);
                SDL_Rect rect = {x1 - thickness / 2, topY, thickness, height};
                SDL_RenderFillRect(gRenderer, &rect);
            } else if (y1 == y2) {
                int leftX = std::min(x1, x2);
                int width = abs(x2 - x1);
                SDL_Rect rect = {leftX, y1 - thickness / 2, width, thickness};
                SDL_RenderFillRect(gRenderer, &rect);
            }

            if (i + 2 < path.size()) {
                int r3 = path[i + 2].first, c3 = path[i + 2].second;
                if (!(r3 == houseRow && c3 == houseCol)) {
                    int x3 = c3 * TILE_SIZE + TILE_SIZE / 2;
                    int y3 = r3 * TILE_SIZE + TILE_SIZE / 2;

                    int dx1 = x2 - x1, dy1 = y2 - y1;
                    int dx2 = x3 - x2, dy2 = y3 - y2;
                    if (dx1 * dy2 - dy1 * dx2 != 0) {
                        SDL_Rect cornerRect = {x2 - thickness / 2, y2 - thickness / 2, thickness, thickness};
                        SDL_RenderFillRect(gRenderer, &cornerRect);
                    }
                }
            }
        }
    }

    renderTexture(rabbitTex, rabbitCol * TILE_SIZE, rabbitRow * TILE_SIZE, TILE_SIZE, TILE_SIZE);
    renderTexture(houseTex, houseCol * TILE_SIZE, houseRow * TILE_SIZE, TILE_SIZE, TILE_SIZE);

    SDL_RenderSetViewport(gRenderer, NULL);

    SDL_Color black = {0, 0, 0, 255};
    std::string timeText = "Time left: " + std::to_string(timeLeft) + "s";
    SDL_Texture* timeTex = renderText(timeText, black);
    int timeTexHeight = 0;
    if (timeTex) {
        int timeTexWidth;
        SDL_QueryTexture(timeTex, NULL, NULL, &timeTexWidth, &timeTexHeight);
        renderTexture(timeTex, 10, 10);
        SDL_DestroyTexture(timeTex);
    }

    // Hiển thị điểm số
    std::string scoreText = "Score: " + std::to_string(score);
    SDL_Texture* scoreTex = renderText(scoreText, black);
    int scoreTexHeight = 0; // Khai báo biến scoreTexHeight
    if (scoreTex) {
        int scoreTexWidth;
        SDL_QueryTexture(scoreTex, NULL, NULL, &scoreTexWidth, &scoreTexHeight);
        renderTexture(scoreTex, 10, 10 + timeTexHeight + 5);
        SDL_DestroyTexture(scoreTex);
    }

    // Hiển thị nút Pause (giả sử lỗi ở đây)
    if (pauseTex) {
        pauseButtonX = 10;
        pauseButtonY = 10 + timeTexHeight + 5 + scoreTexHeight + 5; // Sử dụng scoreTexHeight
        renderTexture(pauseTex, pauseButtonX, pauseButtonY, pauseButtonWidth, pauseButtonHeight);
    }

    if (levelComplete) {
        if (currentLevel < totalLevels) {
            if (nextLevelTex) {
                int tw, th;
                SDL_QueryTexture(nextLevelTex, NULL, NULL, &tw, &th);
                renderTexture(nextLevelTex, gameAreaX, gameAreaY, gameAreaWidth, gameAreaHeight);
            }
        } else {
            if (youWinTex) {
                int tw, th;
                SDL_QueryTexture(youWinTex, NULL, NULL, &tw, &th);
                renderTexture(youWinTex, gameAreaX, gameAreaY, gameAreaWidth, gameAreaHeight);
            }
        }
    }
    if (gameOver) {
        if (gameOverTex) {
            int tw, th;
            SDL_QueryTexture(gameOverTex, NULL, NULL, &tw, &th);
            renderTexture(gameOverTex, gameAreaX, gameAreaY, gameAreaWidth, gameAreaHeight);
        }
    }

    SDL_RenderPresent(gRenderer);
}

void handleGameEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            quit = true;
        } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
            quit = true;
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x = e.button.x;
            int y = e.button.y;

            SDL_Rect pauseButtonRect = {pauseButtonX, pauseButtonY, pauseButtonWidth, pauseButtonHeight};
            if (x >= pauseButtonRect.x && x <= pauseButtonRect.x + pauseButtonRect.w &&
                y >= pauseButtonRect.y && y <= pauseButtonRect.y + pauseButtonRect.h) {
                if (!isPaused) {
                    isPaused = true;
                    pauseStartTime = SDL_GetTicks();
                    Mix_PauseMusic();  // Tạm dừng nhạc khi trò chơi bị tạm dừng
                    std::cout << "Game Paused\n";
                } else {
                    isPaused = false;
                    totalPausedTime += SDL_GetTicks() - pauseStartTime;
                    Mix_ResumeMusic();  // Tiếp tục nhạc khi trò chơi được tiếp tục
                    std::cout << "Game Resumed\n";
                }
            }

            if (!gameOver && !levelComplete && !isPaused) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    path.clear();
                }
            }
        } else if (e.type == SDL_MOUSEMOTION && (e.motion.state & SDL_BUTTON_LMASK)) {
            if (!gameOver && !levelComplete && !isPaused) {
                int col = (e.motion.x - gameAreaX) / TILE_SIZE;
                int row = (e.motion.y - gameAreaY) / TILE_SIZE;
                if (row >= 0 && row < rows && col >= 0 && col < cols && !isObstacle(row, col)) {
                    for (size_t i = 0; i < path.size(); i++) {
                        if (path[i].first == row && path[i].second == col) {
                            path.erase(path.begin() + i, path.end());
                            break;
                        }
                    }
                    if (path.empty() || path.back() != std::make_pair(row, col)) {
                        path.push_back({row, col});
                    }
                }
            }
        } else if (e.type == SDL_MOUSEBUTTONUP) {
            if (!gameOver && !levelComplete && !isPaused && checkWin()) {
                levelComplete = true;
                score += 10;
            }
        }
    }
}

void gameLoop() {
    levelComplete = false;
    gameOver = false;
    path.clear();
    isPaused = false;
    totalPausedTime = 0;

    startTime = SDL_GetTicks();
    timeLeft = GAME_TIME;

    if (musicOn && !musicPlaying) {
    Mix_PlayMusic(gameMusic, -1);
    musicPlaying = true;
}

    levelManager.loadLevel(currentLevel);
    const Level& level = levelManager.getCurrentLevel();

    rabbitRow = level.rabbitPos.first;
    rabbitCol = level.rabbitPos.second;
    houseRow = level.housePos.first;
    houseCol = level.housePos.second;
    obstacles = level.obstacles;
    bridges = level.bridges;
    rows = level.rows;
    cols = level.cols;

    int margin = 20;
    int maxTileWidth = (SCREEN_WIDTH - 2 * margin) / cols;
    int maxTileHeight = (SCREEN_HEIGHT - 2 * margin) / rows;
    TILE_SIZE = std::min(maxTileWidth, maxTileHeight);

    gameAreaWidth = cols * TILE_SIZE;
    gameAreaHeight = rows * TILE_SIZE;
    gameAreaX = (SCREEN_WIDTH - gameAreaWidth) / 2;
    gameAreaY = (SCREEN_HEIGHT - gameAreaHeight) / 2;

    while (!quit && !levelComplete && !gameOver) {
        Uint32 currentTime = SDL_GetTicks();
        if (!isPaused) {
            float elapsed = (currentTime - startTime - totalPausedTime) / 1000.0f;
            if (elapsed >= GAME_TIME) {
                gameOver = true;
                Mix_HaltMusic();
                musicPaused = true;
                if (musicOn) Mix_PlayMusic(loseMusic, 0);
            } else {
                timeLeft = GAME_TIME - (int)elapsed;
            }
        }

        handleGameEvents();
        renderGame();
        SDL_Delay(1000 / FPS);
    }

    if (levelComplete || gameOver) {
    if (levelComplete && currentLevel < totalLevels) {
        SDL_Delay(2000); // Chờ 2 giây trước khi sang level tiếp theo
    } else {
        bool waiting = true;
        while (waiting && !quit) {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_KEYDOWN) {
                    waiting = false;
                }
            }
            renderGame();
            SDL_RenderPresent(gRenderer);
            SDL_Delay(1000 / FPS);
        }
    }
}
}
