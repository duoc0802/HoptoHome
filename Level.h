// Level.h
#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <utility>

struct Level {
    std::pair<int, int> rabbitPos;  // Vị trí thỏ
    std::pair<int, int> housePos;   // Vị trí nhà
    std::vector<std::pair<int, int>> obstacles;  // Danh sách vật cản
    std::vector<std::pair<int, int>> bridges;    // Danh sách cây cầu
    int rows;  // Số hàng của lưới
    int cols;  // Số cột của lưới
};

class LevelManager {
private:
    Level currentLevel;

public:
    LevelManager();
    void loadLevel(int levelNumber);
    const Level& getCurrentLevel() const;
};

#endif // LEVEL_H
