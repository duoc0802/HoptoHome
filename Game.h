#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>
#include <set>
#include "Level.h" // Thêm include Level.h

// Khai báo biến toàn cục
extern LevelManager levelManager;
extern int currentLevel;

// Các biến trò chơi (cập nhật để sử dụng dữ liệu từ Level)
extern int rabbitRow, rabbitCol;       // Tọa độ của thỏ
extern int houseRow, houseCol;         // Tọa độ của nhà
extern SDL_Color rabbitColor;          // Màu của thỏ
extern SDL_Color houseColor;           // Màu của nhà
extern std::vector<std::pair<int, int>> obstacles; // Danh sách vật cản
extern std::vector<std::pair<int, int>> bridges;   // Danh sách cây cầu

// Các hàm
void gameLoop();

#endif // GAME_H

