#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <utility> // cho std::pair
#include <SDL.h>   // cho SDL_Color

// Cấu trúc dữ liệu cho một Level
struct Level {
    std::pair<int, int> rabbitPos;         // Tọa độ của thỏ
    std::pair<int, int> housePos;          // Tọa độ của nhà
    SDL_Color rabbitColor;                 // Màu của thỏ
    SDL_Color houseColor;                  // Màu của nhà
    std::vector<std::pair<int, int>> obstacles; // Danh sách tọa độ vật cản
    std::vector<std::pair<int, int>> bridges;   // Danh sách tọa độ cây cầu
};

// Class quản lý các level
class LevelManager {
public:
    LevelManager();
    void loadLevel(int levelNumber);       // Tải dữ liệu cho level cụ thể
    const Level& getCurrentLevel() const;  // Lấy thông tin level hiện tại

private:
    Level currentLevel;                    // Level đang chơi
};

#endif // LEVEL_H
