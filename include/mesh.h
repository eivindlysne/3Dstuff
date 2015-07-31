#ifndef OKAPI_MESH_H
#define OKAPI_MESH_H

#include <GL/glew.h>

#include "vertex.h"

typedef struct {
    GLuint vao;
    GLuint vbo;
    GLuint ibo;
    size_t size;
} Mesh;

Mesh* Mesh_init(
    Vertex* vertices,
    size_t num_vertices,
    unsigned short* indices,
    size_t num_indices
);

void Mesh_destroy(Mesh* mesh);

void Mesh_draw(Mesh* mesh);

#endif
