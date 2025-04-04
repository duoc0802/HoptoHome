// Menu.h
#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <vector>

enum MenuButton { BTN_START, BTN_MUSIC, BTN_HELP }; //enum cho phép gán tên có nghĩa cho một danh sách các hằng số nguyên

struct Button {
    SDL_Rect box;
    MenuButton type;
};

extern std::vector<Button> menuButtons;

void initMenuButtons();
void handleMenuClick(int x, int y);
void renderMenu();
void menuLoop();

#endif
