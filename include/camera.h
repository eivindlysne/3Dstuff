#ifndef OKAPI_CAMERA_H
#define OKAPI_CAMERA_H

#include <math.h>

#include "linmath/linmath.h"

#include "transform.h"
#include "input.h"

static vec3 G_FORWARD  = { 0.0f,  0.0f, -1.0f};
static vec3 G_BACKWARD = { 0.0f,  0.0f,  1.0f};
static vec3 G_LEFT     = {-1.0f,  0.0f,  0.0f};
static vec3 G_RIGHT    = { 1.0f,  0.0f,  0.0f};
static vec3 G_UP       = { 0.0f,  1.0f,  0.0f};
static vec3 G_DOWN     = { 0.0f, -1.0f,  0.0f};

typedef struct {
    mat4x4 projection;
    Transform transform;
} Camera;

Camera* Camera_init(
    vec3 const position,
    float fov,
    float aspect,
    float zNear,
    float zFar
);

void Camera_destroy(Camera* const camera);

void Camera_update(Camera* const camera, Input* const input);

void Camera_look_at(Camera* const camera, vec3 const position, vec3 const up);

void Camera_offset_orientation(Camera* const camera, float yaw, float pitch);

void Camera_relative_directions(
    Camera* const camera,
    vec3 forward,
    vec3 backward,
    vec3 left,
    vec3 right
);

void Camera_view_projection(Camera* const camera, mat4x4 matrix);

#endif
