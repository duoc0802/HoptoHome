#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <set>

// ------------------------ CÁC HẰNG SỐ ------------------------
const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 640;
const int GRID_SIZE     = 5;
const int TILE_SIZE     = 128;   // Mỗi ô lưới 128x128
const int FPS           = 60;
const int GAME_TIME     = 30;    // 30 giây

// ------------------------ BIẾN TOÀN CỤC ------------------------
SDL_Window*   gWindow   = nullptr;
SDL_Renderer* gRenderer = nullptr;
TTF_Font*     gFont     = nullptr;

// Texture
SDL_Texture* bgMenuTex   = nullptr; // Ảnh nền menu
SDL_Texture* bgGameTex   = nullptr; // Ảnh nền game
SDL_Texture* rabbitTex   = nullptr;
SDL_Texture* houseTex    = nullptr;
SDL_Texture* musicOnTex  = nullptr;
SDL_Texture* musicOffTex = nullptr;
SDL_Texture* helpTex     = nullptr;
SDL_Texture* bushTex     = nullptr;
SDL_Texture* rockTex     = nullptr;

// Âm thanh
Mix_Music* menuMusic = nullptr;   // music1.wav – nhạc menu
Mix_Music* gameMusic = nullptr;   // music2.wav – nhạc game
Mix_Music* loseMusic = nullptr;   // music3.wav – nhạc thua

// Trạng thái
bool quit         = false;
bool isMenu       = true;     // Đang ở menu
bool showHelp     = false;    // Hiển thị hướng dẫn
bool musicOn      = true;     // Nhạc bật/tắt
bool levelComplete= false;
bool gameOver     = false;    // Thua game
int  currentLevel = 1;
int  totalLevels  = 1;        // Ở đây ví dụ 1 level
Uint32 startTime  = 0;        // Thời điểm bắt đầu game
int  timeLeft     = GAME_TIME; // Đếm ngược
bool musicPaused = false;     // Theo dõi nhạc đang bị pause hay không

// Tọa độ thỏ, nhà
int rabbitRow = 0, rabbitCol = 0;
int houseRow  = 1, houseCol = 2;

// Vật cản (bush, rock) (ví dụ)
std::vector<std::pair<int,int>> obstacles = {};

// Lưu đường đi
std::vector<std::pair<int,int>> path;

// ------------------------ HÀM TIỆN ÍCH ------------------------

// **Tạo texture từ file**
SDL_Texture* loadTexture(const std::string &path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cout << "IMG_Load Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_FreeSurface(surface);
    return tex;
}

// **Vẽ texture lên màn hình**
void renderTexture(SDL_Texture* tex, int x, int y, int w = 0, int h = 0) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    if (w == 0 || h == 0) {
        SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    } else {
        dst.w = w;
        dst.h = h;
    }
    SDL_RenderCopy(gRenderer, tex, NULL, &dst);
}

// **Vẽ text bằng TTF**
SDL_Texture* renderText(const std::string &message, SDL_Color color) {
    SDL_Surface* surf = TTF_RenderUTF8_Blended(gFont, message.c_str(), color);
    if (!surf) {
        std::cout << "TTF_RenderUTF8_Blended Error: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surf);
    SDL_FreeSurface(surf);
    return texture;
}

// **Vẽ đường dày**
void drawThickLine(int x1, int y1, int x2, int y2, int thickness) {
    for (int i = -thickness / 2; i <= thickness / 2; ++i) {
        SDL_RenderDrawLine(gRenderer, x1 + i, y1, x2 + i, y2);
        SDL_RenderDrawLine(gRenderer, x1, y1 + i, x2, y2 + i);
    }
}

// ------------------------ KHỞI TẠO SDL ------------------------
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

// ------------------------ TẢI DỮ LIỆU ------------------------
bool loadMedia() {
    gFont = TTF_OpenFont("font.ttf", 28);
    if (!gFont) {
        std::cout << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        return false;
    }

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
        !musicOffTex || !helpTex || !bushTex || !rockTex) {
        std::cout << "Failed to load textures.\n";
        return false;
    }

    menuMusic   = Mix_LoadMUS("music1.wav");
    gameMusic   = Mix_LoadMUS("music2.wav");
    loseMusic   = Mix_LoadMUS("music3.wav");

    if (!menuMusic || !gameMusic || !loseMusic) {
        std::cout << "Failed to load sounds.\n";
        return false;
    }

    return true;
}

// ------------------------ GIẢI PHÓNG ------------------------
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
    bgMenuTex = bgGameTex = rabbitTex = houseTex = musicOnTex = musicOffTex = helpTex = bushTex = rockTex = nullptr;

    Mix_FreeMusic(menuMusic);
    Mix_FreeMusic(gameMusic);
    Mix_FreeMusic(loseMusic);
    menuMusic = gameMusic = loseMusic = nullptr;

    TTF_CloseFont(gFont);
    gFont = nullptr;

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = nullptr;
    gWindow = nullptr;

    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

// ------------------------ MENU: 3 NÚT ------------------------
enum MenuButton { BTN_START, BTN_MUSIC, BTN_HELP };
struct Button {
    SDL_Rect box;
    MenuButton type;
};

std::vector<Button> menuButtons;

void initMenuButtons() {
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
                    if (musicOn)
                        Mix_PlayMusic(gameMusic, -1);
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
            int centerX = btn.box.x + (btn.box.w - tw) / 2;
            int centerY = btn.box.y + (btn.box.h - th) / 2;
            renderTexture(textTex, centerX, centerY);
            SDL_DestroyTexture(textTex);
        }
    }

    if (showHelp) {
        renderTexture(helpTex, 50, 50, SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100);
    }
}

// ------------------------ GAME ------------------------

// **Kiểm tra va chạm chướng ngại**
bool isObstacle(int row, int col) {
    for (auto &obs : obstacles) {
        if (obs.first == row && obs.second == col) return true;
    }
    return false;
}

// **Kiểm tra thắng**
bool checkWin() {
    std::set<std::pair<int, int>> uniquePath(path.begin(), path.end());
    size_t totalCells = GRID_SIZE * GRID_SIZE - obstacles.size();

    return uniquePath.size() == totalCells &&
           path.front() == std::make_pair(rabbitRow, rabbitCol) &&
           path.back() == std::make_pair(houseRow, houseCol);
}

// **Vẽ lưới ô đen**
void renderGrid() {
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    for (int r = 0; r <= GRID_SIZE; r++) {
        SDL_RenderDrawLine(gRenderer, 0, r * TILE_SIZE, SCREEN_WIDTH, r * TILE_SIZE);
    }
    for (int c = 0; c <= GRID_SIZE; c++) {
        SDL_RenderDrawLine(gRenderer, c * TILE_SIZE, 0, c * TILE_SIZE, SCREEN_HEIGHT);
    }
}

// **Vẽ game**
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

    // Vẽ ô thỏ
    SDL_Rect rabbitRect = {rabbitCol * TILE_SIZE, rabbitRow * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255); // Màu xanh lá
    SDL_RenderFillRect(gRenderer, &rabbitRect);
    renderTexture(rabbitTex, rabbitCol * TILE_SIZE, rabbitRow * TILE_SIZE, TILE_SIZE, TILE_SIZE);

    // Vẽ ô nhà
    SDL_Rect houseRect = {houseCol * TILE_SIZE, houseRow * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255); // Màu xanh lá
    SDL_RenderFillRect(gRenderer, &houseRect);
    renderTexture(houseTex, houseCol * TILE_SIZE, houseRow * TILE_SIZE, TILE_SIZE, TILE_SIZE);

    // Vẽ đường dày
    SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255); // Màu xanh lá
    int thickness = 10; // Độ dày đường
    for (size_t i = 0; i + 1 < path.size(); i++) {
        int r1 = path[i].first, c1 = path[i].second;
        int r2 = path[i + 1].first, c2 = path[i + 1].second;
        int x1 = c1 * TILE_SIZE + TILE_SIZE / 2;
        int y1 = r1 * TILE_SIZE + TILE_SIZE / 2;
        int x2 = c2 * TILE_SIZE + TILE_SIZE / 2;
        int y2 = r2 * TILE_SIZE + TILE_SIZE / 2;
        drawThickLine(x1, y1, x2, y2, thickness);
    }

    // Vẽ thời gian còn lại
    SDL_Color red = {255, 0, 0, 255};
    std::string timeText = "Time left: " + std::to_string(timeLeft) + "s";
    SDL_Texture* timeTex = renderText(timeText, red);
    if (timeTex) {
        renderTexture(timeTex, 10, 10);
        SDL_DestroyTexture(timeTex);
    }

    // Nếu thua
    if (gameOver) {
        SDL_Color white = {255, 255, 255, 255};
        std::string overText = "GAME OVER!";
        SDL_Texture* overTex = renderText(overText, white);
        if (overTex) {
            int tw, th;
            SDL_QueryTexture(overTex, NULL, NULL, &tw, &th);
            renderTexture(overTex, (SCREEN_WIDTH - tw) / 2, (SCREEN_HEIGHT - th) / 2);
            SDL_DestroyTexture(overTex);
        }
    }

    // Nếu thắng
    if (levelComplete) {
        SDL_Color white = {255, 255, 255, 255};
        std::string winText = "YOU WIN!";
        SDL_Texture* winTex = renderText(winText, white);
        if (winTex) {
            int tw, th;
            SDL_QueryTexture(winTex, NULL, NULL, &tw, &th);
            renderTexture(winTex, (SCREEN_WIDTH - tw) / 2, (SCREEN_HEIGHT - th) / 2);
            SDL_DestroyTexture(winTex);
        }
    }

    SDL_RenderPresent(gRenderer);
}

// **Xử lý sự kiện game**
void handleGameEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            quit = true;
        } else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (!gameOver && !levelComplete) {
                path.clear();
            }
        } else if (e.type == SDL_MOUSEMOTION) {
            if (!gameOver && !levelComplete && (e.motion.state & SDL_BUTTON_LMASK)) {
                int col = e.motion.x / TILE_SIZE;
                int row = e.motion.y / TILE_SIZE;
                if (row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE) {
                    if (!isObstacle(row, col)) {
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
            }
        } else if (e.type == SDL_MOUSEBUTTONUP) {
            if (!gameOver && !levelComplete) {
                if (checkWin()) {
                    levelComplete = true;
                }
            }
        }
    }
}

// **Game loop**
void gameLoop() {
    levelComplete = false;
    gameOver = false;
    path.clear();

    startTime = SDL_GetTicks();
    timeLeft  = GAME_TIME;

    rabbitRow = 0; rabbitCol = 0;
    houseRow  = 4; houseCol = 4;

    if (musicOn) {
        Mix_PlayMusic(gameMusic, -1);
    }

    while (!quit && !levelComplete && !gameOver) {
        Uint32 currentTime = SDL_GetTicks();
        float elapsed = (currentTime - startTime) / 1000.0f;
        if (elapsed >= GAME_TIME) {
            gameOver = true;
            Mix_HaltMusic();
            musicPaused = true;
            if (musicOn) {
                Mix_PlayMusic(loseMusic, 0);
            }
        } else {
            timeLeft = GAME_TIME - (int)elapsed;
        }

        handleGameEvents();
        renderGame();
        SDL_Delay(1000 / FPS);
    }

    if (levelComplete) {
        SDL_Delay(2000);
    }
    if (gameOver) {
        SDL_Delay(2000);
    }
}

// ------------------------ MAIN LOOP ------------------------
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

    if (musicOn && isMenu) {
        Mix_PlayMusic(menuMusic, -1);
    }

    while (!quit) {
        if (isMenu) {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                    int mx = e.button.x;
                    int my = e.button.y;
                    handleMenuClick(mx, my);
                }
            }
            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
            SDL_RenderClear(gRenderer);
            renderMenu();
            SDL_RenderPresent(gRenderer);
            SDL_Delay(1000 / FPS);
        } else {
            gameLoop();
            isMenu = true;
        }
    }

    closeSDL();
    return 0;
}
