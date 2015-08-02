
#include <SDL2/SDL.h>

#include "input.h"


Input* Input_init() {

    Input* input = malloc(sizeof(Input));

    input->forward_key = false;
    input->back_key = false;
    input->left_key = false;
    input->right_key = false;
    input->mouse_dx = 0.f;
    input->mouse_dy = 0.f;
    input->mouse_sensitivity = 0.05f;

    return input;
}

void Input_destroy(Input* input) {
    free(input);
}

void Input_update(
    Input* const input,
    Display* const display,
    bool* const running
) {

    SDL_Event event;
    while (SDL_PollEvent(&event)) {

        if (event.type == SDL_QUIT) {
            *running = false;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    *running = false;
                    break;
                case SDLK_UP: case SDLK_w:
                    input->forward_key = true;
                    break;
                case SDLK_DOWN: case SDLK_s:
                    input->back_key = true;
                    break;
                case SDLK_LEFT: case SDLK_a:
                    input->left_key = true;
                    break;
                case SDLK_RIGHT: case SDLK_d:
                    input->right_key = true;
                    break;
                default: break;
            }
        } else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
                case SDLK_UP: case SDLK_w:
                    input->forward_key = false;
                    break;
                case SDLK_DOWN: case SDLK_s:
                    input->back_key = false;
                    break;
                case SDLK_LEFT: case SDLK_a:
                    input->left_key = false;
                    break;
                case SDLK_RIGHT: case SDLK_d:
                    input->right_key = false;
                    break;
                default: break;
            }
        }
    }

    const float center_x = 960.f / 2.f;
    const float center_y = 540.f / 2.f;

    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    input->mouse_dx = mouse_x - center_x;
    input->mouse_dy = mouse_y - center_y;

    //SDL_WarpMouseInWindow(display->window, center_x, center_y);
}
