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
LevelManager levelManager;
int currentLevel = 1;
int rabbitRow, rabbitCol;
int houseRow, houseCol;
SDL_Color rabbitColor;
SDL_Color houseColor;
std::vector<std::pair<int, int>> obstacles;
std::vector<std::pair<int, int>> bridges;

// Thêm lại các biến cũ nếu cần
std::vector<std::pair<int, int>> path;
bool levelComplete = false;
bool gameOver = false;
Uint32 startTime = 0;
int timeLeft = GAME_TIME;

bool isObstacle(int row, int col) {
    for (auto &obs : obstacles) {
        if (obs.first == row && obs.second == col) return true;
    }
    return false;
}

bool checkWin() {
    std::set<std::pair<int, int>> uniquePath(path.begin(), path.end());
    size_t totalCells = GRID_SIZE * GRID_SIZE - obstacles.size();
    return uniquePath.size() == totalCells &&
           path.front() == std::make_pair(rabbitRow, rabbitCol) &&
           path.back() == std::make_pair(houseRow, houseCol);
}

void renderGrid() {
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    for (int r = 0; r <= GRID_SIZE; r++) {
        SDL_RenderDrawLine(gRenderer, 0, r * TILE_SIZE, SCREEN_WIDTH, r * TILE_SIZE);
    }
    for (int c = 0; c <= GRID_SIZE; c++) {
        SDL_RenderDrawLine(gRenderer, c * TILE_SIZE, 0, c * TILE_SIZE, SCREEN_HEIGHT);
    }
}

void renderGame() {
    renderTexture(bgGameTex, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    renderGrid();

    for (int r = 0; r < GRID_SIZE; r++) {
        for (int c = 0; c < GRID_SIZE; c++) {
            if (isObstacle(r, c)) {
                renderTexture(bushTex, c * TILE_SIZE, r * TILE_SIZE, TILE_SIZE, TILE_SIZE);
            }
        }
    }

    SDL_Rect rabbitRect = {rabbitCol * TILE_SIZE, rabbitRow * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
    SDL_RenderFillRect(gRenderer, &rabbitRect);
    renderTexture(rabbitTex, rabbitCol * TILE_SIZE, rabbitRow * TILE_SIZE, TILE_SIZE, TILE_SIZE);

    SDL_Rect houseRect = {houseCol * TILE_SIZE, houseRow * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
    SDL_RenderFillRect(gRenderer, &houseRect);
    renderTexture(houseTex, houseCol * TILE_SIZE, houseRow * TILE_SIZE, TILE_SIZE, TILE_SIZE);

    SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
    int thickness = 10;
    for (size_t i = 0; i + 1 < path.size(); i++) {
        int r1 = path[i].first, c1 = path[i].second;
        int r2 = path[i + 1].first, c2 = path[i + 1].second;
        int x1 = c1 * TILE_SIZE + TILE_SIZE / 2;
        int y1 = r1 * TILE_SIZE + TILE_SIZE / 2;
        int x2 = c2 * TILE_SIZE + TILE_SIZE / 2;
        int y2 = r2 * TILE_SIZE + TILE_SIZE / 2;
        drawThickLine(x1, y1, x2, y2, thickness);
    }

    SDL_Color red = {255, 0, 0, 255};
    std::string timeText = "Time left: " + std::to_string(timeLeft) + "s";
    SDL_Texture* timeTex = renderText(timeText, red);
    if (timeTex) {
        renderTexture(timeTex, 10, 10);
        SDL_DestroyTexture(timeTex);
    }

    if (gameOver) {
        SDL_Color white = {255, 255, 255, 255};
        SDL_Texture* overTex = renderText("GAME OVER!", white);
        if (overTex) {
            int tw, th;
            SDL_QueryTexture(overTex, NULL, NULL, &tw, &th);
            renderTexture(overTex, (SCREEN_WIDTH - tw) / 2, (SCREEN_HEIGHT - th) / 2);
            SDL_DestroyTexture(overTex);
        }
    }

    if (levelComplete) {
        SDL_Color white = {255, 255, 255, 255};
        SDL_Texture* winTex = renderText("YOU WIN!", white);
        if (winTex) {
            int tw, th;
            SDL_QueryTexture(winTex, NULL, NULL, &tw, &th);
            renderTexture(winTex, (SCREEN_WIDTH - tw) / 2, (SCREEN_HEIGHT - th) / 2);
            SDL_DestroyTexture(winTex);
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
            if (!gameOver && !levelComplete) path.clear();
        } else if (e.type == SDL_MOUSEMOTION && (e.motion.state & SDL_BUTTON_LMASK)) {
            if (!gameOver && !levelComplete) {
                int col = e.motion.x / TILE_SIZE;
                int row = e.motion.y / TILE_SIZE;
                if (row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE && !isObstacle(row, col)) {
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
            if (!gameOver && !levelComplete && checkWin()) {
                levelComplete = true;
            }
        }
    }
}

void gameLoop() {
    levelComplete = false;
    gameOver = false;
    path.clear();

    startTime = SDL_GetTicks();
    timeLeft  = GAME_TIME;

    if (musicOn) Mix_PlayMusic(gameMusic, -1);
    // Tải level hiện tại
    levelManager.loadLevel(currentLevel);
    const Level& level = levelManager.getCurrentLevel();

    // Gán dữ liệu từ level
    rabbitRow = level.rabbitPos.first;
    rabbitCol = level.rabbitPos.second;
    houseRow = level.housePos.first;
    houseCol = level.housePos.second;
    rabbitColor = level.rabbitColor;
    houseColor = level.houseColor;
    obstacles = level.obstacles;
    bridges = level.bridges;

    // Logic cũ được giữ lại
    // Ví dụ: vẽ, xử lý input, kiểm tra thắng thua

    while (!quit && !levelComplete && !gameOver) {
        Uint32 currentTime = SDL_GetTicks();
        float elapsed = (currentTime - startTime) / 1000.0f;
        if (elapsed >= GAME_TIME) {
            gameOver = true;
            Mix_HaltMusic();
            musicPaused = true;
            if (musicOn) Mix_PlayMusic(loseMusic, 0);
        } else {
            timeLeft = GAME_TIME - (int)elapsed;
        }

        handleGameEvents();
        renderGame();
        SDL_Delay(1000 / FPS);
    }

    if (levelComplete || gameOver) SDL_Delay(2000);
}

