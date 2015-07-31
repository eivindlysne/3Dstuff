#include <stdio.h>
#include <stdbool.h>

#include "display.h"

Display* Display_init(char* title, int width, int height) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Failed to initialize SDL\n");
        return NULL;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    //SDL_GL_SetSwapInterval(0); // vsync off if driver allows
    //SDL_SetRelativeMouseMode(true); // grab mouse

    SDL_Window* window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(window);

    Display* display = malloc(sizeof(Display));
    display->window = window;
    display->context = context;
    return display;
}

void Display_destroy(Display* display) {

    SDL_GL_DeleteContext(display->context);
    SDL_Quit();
    free(display);
}
