#ifndef OKAPI_TRANSFORM_H
#define OKAPI_TRANSFORM_H

#include "linmath/linmath.h"

#ifndef M_PI
    #define M_PI 3.1415926535
#endif
#define toRadians(angleDegrees) (angleDegrees * M_PI / 180.0)
#define toDegrees(angleRadians) (angleRadians * 180.0 / M_PI)


typedef struct {
    vec3 position;
    quat orientation;
    vec3 scale;
} Transform;

Transform Transform_init(
    vec3 const position,
    quat const orientation,
    vec3 const scale
);

Transform Transform_init_default();

void Transform_destroy(Transform* const transform);

void Transform_model_matrix(Transform* const transform, mat4x4 matrix);

#endif
