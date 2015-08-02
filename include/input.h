#ifndef OKAPI_INPUT_H
#define OKAPI_INPUT_H

#include <stdbool.h>

#include "display.h"


typedef struct {
    bool forward_key;
    bool back_key;
    bool left_key;
    bool right_key;
    float mouse_dx;
    float mouse_dy;
    float mouse_sensitivity;
} Input;

Input* Input_init();

void Input_destroy(Input* input);

void Input_update(
    Input* const input,
    Display* const display,
    bool* const running
);

#endif
