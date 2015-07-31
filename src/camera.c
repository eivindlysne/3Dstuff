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

    return camera;
}

void Camera_destroy(Camera* const camera) {
    free(camera);
}

// TODO: Might need
void Camera_look_at(Camera* const camera, vec3 const position, vec3 const forward) {

    // float const similar = 0.001f;
    //
    // vec3 position_diff;
    // vec3_sub(position_diff, position, camera->transform.position);
    //
    // if (vec3_len(position_diff) < similar)
    //     return; // You cannot look at where you are!
    //
    // vec3 f;
    // vec3_norm(f, position_diff);
    // float cosTheta = vec3_mul_inner(forward, f); // dot
    //
    // float angle = acos(cosTheta);
    // vec3 axis;
    // vec3_mul_cross(axis, forward, f);
    // vec3_norm(axis, axis);
    //
    // quat q; //quat_identity(q);
    // quat_rotate(q, angle, axis);
    // camera->transform.orientation[0] = q[0];
    // camera->transform.orientation[1] = q[1];
    // camera->transform.orientation[2] = q[2];
    // camera->transform.orientation[3] = q[3];
}

// TODO: Does not work properly (all values in orientation go toward 0)
void Camera_offset_orientation(Camera* const camera, float yaw, float pitch) {

    quat pitch_rotation, yaw_rotation;

    quat_rotate(yaw_rotation, yaw, G_UP);

    vec3 right;
    quat_mul_vec3(right, camera->transform.orientation, G_RIGHT);
    vec3_norm(right, right);
    quat_rotate(pitch_rotation, pitch, right);

    quat orientation;
    quat_mul(orientation, yaw_rotation, pitch_rotation);
    quat_mul(
        camera->transform.orientation,
        orientation,
        camera->transform.orientation
    );
    quat_norm(camera->transform.orientation, camera->transform.orientation);
}

void Camera_relative_directions(
    Camera* const camera,
    vec3 forward,
    vec3 backward,
    vec3 left,
    vec3 right
) {
    quat_mul_vec3(forward, camera->transform.orientation, G_FORWARD);
    quat_mul_vec3(backward, camera->transform.orientation, G_BACKWARD);
    quat_mul_vec3(left, camera->transform.orientation, G_LEFT);
    quat_mul_vec3(right, camera->transform.orientation, G_RIGHT);
}

void Camera_view_projection(Camera* const camera, mat4x4 matrix) {

    mat4x4 position_matrix, orientation_matrix, scale_matrix;

    mat4x4_translate(
        position_matrix,
        -camera->transform.position[0],
        -camera->transform.position[1],
        -camera->transform.position[2]
    );

    quat q;
    quat_conj(q, camera->transform.orientation);
    mat4x4_from_quat(orientation_matrix, q);

    vec3 v = {
        1.0f / camera->transform.scale[0],
        1.0f / camera->transform.scale[1],
        1.0f / camera->transform.scale[2]
    };
    mat4x4 m;
    mat4x4_identity(m);
    mat4x4_scale_aniso(scale_matrix, m, v[0], v[1], v[2]);

    mat4x4_mul(matrix, scale_matrix, orientation_matrix);
    mat4x4_mul(matrix, matrix, position_matrix);

    mat4x4_mul(matrix, camera->projection, matrix);
}
