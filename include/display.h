#ifndef OKAPI_DISPLAY_H
#define OKAPI_DISPLAY_H

#include <SDL2/SDL.h>


typedef struct {
    SDL_Window* window;
    SDL_GLContext context;
    unsigned int width;
    unsigned int height;
} Display;

Display* Display_init(char* title, unsigned int width, unsigned int height);

void Display_destroy(Display* display);

#endif
