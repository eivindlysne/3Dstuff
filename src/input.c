
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
    Display* const display,
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
        }
    }

    const float mouse_sensitivity = 0.05f;
    const float center_x = 960.f / 2.f;
    const float center_y = 540.f / 2.f;

    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    float mouse_dx = mouse_x - center_x;
    float mouse_dy = mouse_y - center_y;

    Camera_offset_orientation(
        camera,
        mouse_sensitivity * toRadians((-mouse_dx)),
        mouse_sensitivity * toRadians((-mouse_dy))
    );

    //SDL_WarpMouseInWindow(display->window, center_x, center_y);

    // TODO: Move input "handling"
    // TODO: Delta time
    vec3 dir = {0.0f, 0.0f, 0.0f};
    float vel = 0.2f;
    vec3 forward, backward, left, right;
    Camera_relative_directions(camera, forward, backward, left, right);

    if (input->forward_key) {
        vec3 f = {forward[0], 0.0f, forward[2]};
        vec3_norm(f, f);
        vec3_add(dir, dir, f);
    }
    if (input->back_key) {
        vec3 b = {backward[0], 0.0f, backward[2]};
        vec3_norm(b, b);
        vec3_add(dir, dir, b);
    }
    if (input->left_key) {
        vec3_add(dir, dir, left);
    }
    if (input->right_key) {
        vec3_add(dir, dir, right);
    }

    if (vec3_len(dir) > 0.0f) {
        vec3_norm(dir, dir);
    }

    vec3 movement;
    vec3_scale(movement, dir, vel);
    vec3_add(camera->transform.position, camera->transform.position, movement);
}
