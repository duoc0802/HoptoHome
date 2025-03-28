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
extern std::vector<std::pair<int, int>> obstacles; // Danh sách vật cản
extern std::vector<std::pair<int, int>> bridges;   // Danh sách cây cầu
extern int gameAreaWidth, gameAreaHeight;          // Kích thước khu vực chơi
extern int gameAreaX, gameAreaY;                   // Tọa độ khu vực chơi
extern int score;                                  // Điểm số
extern int TILE_SIZE;                              // Kích thước ô
extern std::vector<std::pair<int, int>> path;      // Đường đi
extern bool levelComplete;                         // Trạng thái hoàn thành cấp độ
extern bool gameOver;                              // Trạng thái kết thúc trò chơi
extern Uint32 startTime;                           // Thời gian bắt đầu
extern int timeLeft;                               // Thời gian còn lại

// Các hàm
void gameLoop();

#endif // GAME_H
