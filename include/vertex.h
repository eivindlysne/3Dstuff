#ifndef OKAPI_VERTEX_H
#define OKAPI_VERTEX_H

#include "linmath/linmath.h"

#define POSITION_SIZE 3
#define TEXCOORD_SIZE 2

typedef struct {
    vec3 position;
    vec2 tex_coord;
} Vertex;

typedef struct {
    vec3 position;
} SkyboxVertex;

#endif
