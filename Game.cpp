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
int gameAreaSize;  // Kích thước khu vực chơi (vuông)
int gameAreaX, gameAreaY;  // Vị trí khu vực chơi
int score = 0;  // Điểm số ban đầu
int gameAreaHeight = 5;   // Định nghĩa: 5 hàng trong lưới
int gameAreaWidth = 7;    // Định nghĩa: 7 cột trong lưới
int TILE_SIZE = 32;       // Định nghĩa: Kích thước mỗi ô là 32 pixel
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
    size_t totalCells = ROWS * COLS - obstacles.size();
    return uniquePath.size() == totalCells &&
           path.front() == std::make_pair(rabbitRow, rabbitCol) &&
           path.back() == std::make_pair(houseRow, houseCol);
}

void renderGrid() {
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    for (int r = 0; r <= ROWS; r++) {
        SDL_RenderDrawLine(gRenderer, 0, r * TILE_SIZE, gameAreaWidth, r * TILE_SIZE);
    }
    for (int c = 0; c <= COLS; c++) {
        SDL_RenderDrawLine(gRenderer, c * TILE_SIZE, 0, c * TILE_SIZE, gameAreaHeight);
    }
}


void renderGame() {
    // Thiết lập viewport cho khu vực chơi game
    SDL_Rect gameArea = {gameAreaX, gameAreaY, gameAreaWidth, gameAreaHeight};
    SDL_RenderSetViewport(gRenderer, &gameArea);

    // Vẽ nền game trong khu vực chơi
    renderTexture(bgGameTex, 0, 0, gameAreaWidth, gameAreaHeight);

    // Vẽ lưới trong khu vực chơi
    renderGrid();

    // Vẽ chướng ngại vật
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (isObstacle(r, c)) {
                renderTexture(bushTex, c * TILE_SIZE, r * TILE_SIZE, TILE_SIZE, TILE_SIZE);
            }
        }
    }

    // Vẽ đường đi với các hình chữ nhật liền nhau, bo góc vuông vức
    SDL_SetRenderDrawColor(gRenderer, 250, 170, 141, 255);
    int thickness = 10;  // Độ dày của đường đi

    if (path.size() > 1) {  // Chỉ vẽ khi có ít nhất 2 điểm
        for (size_t i = 0; i + 1 < path.size(); i++) {
            int r1 = path[i].first, c1 = path[i].second;
            int r2 = path[i + 1].first, c2 = path[i + 1].second;
            int x1 = c1 * TILE_SIZE + TILE_SIZE / 2;
            int y1 = r1 * TILE_SIZE + TILE_SIZE / 2;
            int x2 = c2 * TILE_SIZE + TILE_SIZE / 2;
            int y2 = r2 * TILE_SIZE + TILE_SIZE / 2;

            // Xác định hướng của đoạn đường (ngang hay dọc)
            if (x1 == x2) {  // Đường dọc
                int topY = std::min(y1, y2);
                int height = abs(y2 - y1);
                SDL_Rect rect = {x1 - thickness / 2, topY, thickness, height};
                SDL_RenderFillRect(gRenderer, &rect);
            } else if (y1 == y2) {  // Đường ngang
                int leftX = std::min(x1, x2);
                int width = abs(x2 - x1);
                SDL_Rect rect = {leftX, y1 - thickness / 2, width, thickness};
                SDL_RenderFillRect(gRenderer, &rect);
            }

            // Vẽ hình chữ nhật vuông tại góc nếu có khúc rẽ
            if (i + 2 < path.size()) {
                int r3 = path[i + 2].first, c3 = path[i + 2].second;
                if (!(r3 == houseRow && c3 == houseCol)) {  // Bỏ qua nếu là nhà
                    int x3 = c3 * TILE_SIZE + TILE_SIZE / 2;
                    int y3 = c3 * TILE_SIZE + TILE_SIZE / 2;

                    // Kiểm tra nếu có khúc rẽ
                    int dx1 = x2 - x1, dy1 = y2 - y1;
                    int dx2 = x3 - x2, dy2 = y3 - y2;
                    if (dx1 * dy2 - dy1 * dx2 != 0) {  // Có khúc rẽ
                        // Vẽ hình chữ nhật vuông tại điểm rẽ
                        SDL_Rect cornerRect = {x2 - thickness / 2, y2 - thickness / 2, thickness, thickness};
                        SDL_RenderFillRect(gRenderer, &cornerRect);
                    }
                }
            }
        }
    }

    // Vẽ nhân vật thỏ
    renderTexture(rabbitTex, rabbitCol * TILE_SIZE, rabbitRow * TILE_SIZE, TILE_SIZE, TILE_SIZE);

    // Vẽ nhà
    renderTexture(houseTex, houseCol * TILE_SIZE, houseRow * TILE_SIZE, TILE_SIZE, TILE_SIZE);

    // Vẽ thời gian còn lại trong khung trò chơi (trước khi reset viewport)
    SDL_Color red = {255, 0, 0, 255};
    std::string timeText = "Time left: " + std::to_string(timeLeft) + "s";
    SDL_Texture* timeTex = renderText(timeText, red);
    if (timeTex) {
        int timeTexWidth, timeTexHeight;
        SDL_QueryTexture(timeTex, NULL, NULL, &timeTexWidth, &timeTexHeight);
        // Đặt ở góc trên bên phải của gameArea
        int x = gameAreaWidth - timeTexWidth - 10; // Cách lề phải 10 pixel
        int y = 10; // Cách lề trên 10 pixel
        renderTexture(timeTex, x, y);
        SDL_DestroyTexture(timeTex);
    }

    // Reset viewport về toàn bộ màn hình
    SDL_RenderSetViewport(gRenderer, NULL);

    // Vẽ thông báo GAME OVER hoặc YOU WIN
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

    // Hiển thị điểm số
    if (gameOver || levelComplete) {
        SDL_Color white = {255, 255, 255, 255};
        std::string scoreText = "Score: " + std::to_string(score);
        SDL_Texture* scoreTex = renderText(scoreText, white);
        if (scoreTex) {
            int tw, th;
            SDL_QueryTexture(scoreTex, NULL, NULL, &tw, &th);
            renderTexture(scoreTex, (SCREEN_WIDTH - tw) / 2, (SCREEN_HEIGHT - th) / 2 + 50);
            SDL_DestroyTexture(scoreTex);
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
                int col = (e.motion.x - gameAreaX) / TILE_SIZE;
                int row = (e.motion.y - gameAreaY) / TILE_SIZE;
                if (row >= 0 && row < ROWS && col >= 0 && col < COLS && !isObstacle(row, col)) {
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
                score += 10;
            }
        }
    }
}

void gameLoop() {
    levelComplete = false;
    gameOver = false;
    path.clear();

    startTime = SDL_GetTicks();
    timeLeft = GAME_TIME;

    if (musicOn) Mix_PlayMusic(gameMusic, -1);

    // Tải level hiện tại
    levelManager.loadLevel(currentLevel);
    const Level& level = levelManager.getCurrentLevel();

    rabbitRow = level.rabbitPos.first;
    rabbitCol = level.rabbitPos.second;
    houseRow = level.housePos.first;
    houseCol = level.housePos.second;
    obstacles = level.obstacles;
    bridges = level.bridges;

    // Tính toán kích thước ô và khu vực chơi
    int margin = 20;  // Khoảng cách lề
    int maxTileWidth = (SCREEN_WIDTH - 2 * margin) / COLS;
    int maxTileHeight = (SCREEN_HEIGHT - 2 * margin) / ROWS;
    TILE_SIZE = std::min(maxTileWidth, maxTileHeight);

    gameAreaWidth = COLS * TILE_SIZE;
    gameAreaHeight = ROWS * TILE_SIZE;
    gameAreaX = (SCREEN_WIDTH - gameAreaWidth) / 2;  // Căn giữa ngang
    gameAreaY = (SCREEN_HEIGHT - gameAreaHeight) / 2;  // Căn giữa dọc

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
