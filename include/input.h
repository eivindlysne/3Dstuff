#ifndef OKAPI_INPUT_H
#define OKAPI_INPUT_H

#include <stdbool.h>

#include "camera.h"

typedef struct {
    bool forward_key;
    bool back_key;
    bool left_key;
    bool right_key;
    int mouse_x;
    int mouse_y;
} Input;

Input* Input_init();

void Input_destroy(Input* input);

void Input_update(Input* const input, Camera* const camera, bool* const running);

#endif
