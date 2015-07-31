#include <stdlib.h>

#include "transform.h"


Transform Transform_init(
    vec3 const position,
    quat const orientation,
    vec3 const scale
) {
    Transform transform = {
        .position = {*position},
        .orientation = {*orientation},
        .scale = {*scale},
    };
    return transform;
}

Transform Transform_init_default() {
    Transform transform = {
        .position = {0.0f, 0.0f, 0.0f},
        .orientation = {0.0f, 0.0f, 0.0f, 1.0f},
        .scale = {1.0f, 1.0f, 1.0f},
    };
    return transform;
}

void Transform_destroy(Transform* const transform) {
    free(transform);
}

void Transform_model_matrix(Transform* const transform, mat4x4 matrix) {

    mat4x4_identity(matrix);

    mat4x4 position_matrix, orientation_matrix, scale_matrix, temp;
    vec3 position = {transform->position[0], transform->position[1], transform->position[2]};
    quat orientation = {
        transform->orientation[0],
        transform->orientation[1],
        transform->orientation[2],
        transform->orientation[3]
    };
    vec3 scale = {transform->scale[0], transform->scale[1], transform->scale[2]};

    mat4x4_translate(position_matrix, position[0], position[1], position[2]);
    mat4x4_from_quat(orientation_matrix, orientation);
    mat4x4_identity(temp);
    mat4x4_scale_aniso(scale_matrix, temp, scale[0], scale[1], scale[2]);

    mat4x4_mul(matrix, orientation_matrix, scale_matrix);
    mat4x4_mul(matrix, position_matrix, matrix);
}
