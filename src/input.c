
#include <SDL2/SDL.h>

#include "input.h"


Input* Input_init() {

    Input* input = malloc(sizeof(Input));

    input->forward_key = false;
    input->back_key = false;
    input->left_key = false;
    input->right_key = false;
    input->mouse_x = 0;
    input->mouse_y = 0;

    return input;
}

void Input_destroy(Input* input) {
    free(input);
}

void Input_update(
    Input* const input,
    Camera* const camera,
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
        } else if (event.type == SDL_MOUSEMOTION) {

            //SDL_GetMouseState(&mouse_x, &mouse_y);

        }
    }


    // TODO: Move input "handling"
    // TODO: Normalize output
    // vec3 dir = {0.0f, 0.0f, 0.0f};
    // vec3 vel = {0.2f, 0.2f, 0.2f};

    if (input->forward_key) {
        camera->position[0] += camera->forward[0] * 0.2f;
        camera->position[2] += camera->forward[2] * 0.2f;
        // camera->transform.position[2] += 0.2f;
        // dir[0] += camera->forward[0];
        // dir[1] += camera->forward[1];
        // dir[2] += camera->forward[2];
    }
    if (input->back_key) {
        camera->position[0] -= camera->forward[0] * 0.2f;
        camera->position[2] -= camera->forward[2] * 0.2f;
        // camera->transform.position[2] -= 0.2f;
        // dir[0] -= camera->forward[0];
        // dir[1] -= camera->forward[1];
        // dir[2] -= camera->forward[2];
    }
    if (input->left_key) {
        camera->position[0] -= 0.2f;
        // camera->transform.position[0] -= 0.2f;
    }
    if (input->right_key) {
        camera->position[0] += 0.2f;
        // camera->transform.position[0] += 0.2f;
    }
}
