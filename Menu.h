#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <vector>

enum MenuButton { BTN_START, BTN_MUSIC, BTN_HELP };

struct Button {
    SDL_Rect box;
    MenuButton type;
};

extern std::vector<Button> menuButtons;

void initMenuButtons();
void handleMenuClick(int x, int y);
void renderMenu();

#endif // MENU_H
