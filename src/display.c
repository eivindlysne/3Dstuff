#include <stdio.h>
#include <stdbool.h>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "display.h"


static void init_OpenGL(Display* const display) {

    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW!");
        return;
    }

#if 0
    printf("----------------------------------------------------------------\n");
    printf("Graphics Successfully Initialized\n");
    printf("OpenGL Info\n");
    printf("    Version: %s\n", glGetString(GL_VERSION));
    printf("     Vendor: %s\n", glGetString(GL_VENDOR));
    printf("   Renderer: %s\n", glGetString(GL_RENDERER));
    printf("    Shading: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("----------------------------------------------------------------\n");
#endif

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glViewport(0.0f, 0.0f, display->width, display->height);

    glEnable(GL_TEXTURE_2D); // Not needed?
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); // Dunno if doin' anything

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glEnable(GL_CULL_FACE); // Cannot rotate?
    //glCullFace(GL_BACK);
}


Display* Display_init(char* title, unsigned int width, unsigned int height) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Failed to initialize SDL\n");
        return NULL;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    //SDL_GL_SetSwapInterval(0); // vsync off if driver allows

    SDL_Window* window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_OPENGL
    );

    //SDL_SetRelativeMouseMode(true); // grab mouse

    SDL_WarpMouseInWindow(
        window,
        (float) width / 2.f,
        (float) height / 2.f
    );

    SDL_GLContext context = SDL_GL_CreateContext(window);

    Display* display = malloc(sizeof(Display));
    display->window = window;
    display->context = context;
    display->width = width;
    display->height = height;

    init_OpenGL(display);

    return display;
}

void Display_destroy(Display* display) {

    SDL_GL_DeleteContext(display->context);
    SDL_Quit();
    free(display);
}
