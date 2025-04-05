#ifndef UTILS_H
#define UTILS_H

#include <SDL.h>
#include <string>

SDL_Texture* loadTexture(const std::string &path);
void renderTexture(SDL_Texture* tex, int x, int y, int w = 0, int h = 0);
SDL_Texture* renderText(const std::string &message, SDL_Color color);
<<<<<<< HEAD

=======
>>>>>>> a35aef98926204bf1ca75f7cdf7acdc3059513b9
#endif // UTILS_H
