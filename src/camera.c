#include <stdlib.h>

#include "camera.h"


Camera* Camera_init(
    vec3 const position,
    float fov,
    float aspect,
    float zNear,
    float zFar
) {
    Camera* camera = malloc(sizeof(Camera));

    camera->transform = Transform_init_default();
    camera->transform.position[0] = position[0];
    camera->transform.position[1] = position[1];
    camera->transform.position[2] = position[2];

    mat4x4_perspective(camera->projection, toRadians(fov), aspect, zNear, zFar);

    camera->position[0] = position[0];
    camera->position[1] = position[1];
    camera->position[2] = position[2];

    camera->forward[0] = 0.0f;
    camera->forward[1] = 0.0f;
    camera->forward[2] = -1.0f;

    camera->up[0] = 0.0f;
    camera->up[1] = 1.0f;
    camera->up[2] = 0.0f;

    return camera;
}

void Camera_destroy(Camera* const camera) {
    free(camera);
}

// TODO: Might need
void Camera_look_at() {}

void Camera_offset_orientation(Camera* const camera, float pitch, float yaw) {

    quat pitch_rotation, yaw_rotation;

    quat_rotate(yaw_rotation, yaw, G_UP);

    vec3 right;
    quat_mul_vec3(right, camera->transform.orientation, G_RIGHT);
    quat_rotate(pitch_rotation, pitch, right);

    quat orientation;
    quat_mul(orientation, yaw_rotation, pitch_rotation);
    quat_mul(
        camera->transform.orientation,
        camera->transform.orientation,
        orientation
    );
}

void Camera_view_projection(Camera* const camera, mat4x4 matrix) {

    // vec3 center;
    // vec3_add(center, camera->position, camera->forward);
    // mat4x4_look_at(matrix, camera->position, center, camera->up);
    // mat4x4_mul(matrix, camera->projection, matrix);

    vec3 center, forward, up;
    quat_mul_vec3(forward, camera->transform.orientation, G_FORWARD);
    quat_mul_vec3(up, camera->transform.orientation, G_UP);

    vec3_add(center, camera->transform.position, forward);
    mat4x4_look_at(matrix, camera->transform.position, center, up);
    mat4x4_mul(matrix, camera->projection, matrix);
}
