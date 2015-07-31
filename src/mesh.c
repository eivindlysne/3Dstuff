#include <stdlib.h>

#include "mesh.h"


Mesh* Mesh_init(Vertex* vertices, size_t num_vertices, unsigned short* indices, size_t num_indices) {

    Mesh* mesh = malloc(sizeof(Mesh));
    mesh->size = num_indices;

    glGenVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);

    glGenBuffers(1, &mesh->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        num_vertices * sizeof(Vertex),
        vertices,
        GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        0,
        POSITION_SIZE,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        0
    );
    glVertexAttribPointer(
        1,
        TEXCOORD_SIZE,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*) (POSITION_SIZE * sizeof(float))
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &mesh->ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        num_indices * sizeof(unsigned short),
        indices,
        GL_STATIC_DRAW
    );

    glBindVertexArray(0);

    return mesh;
}

void Mesh_destroy(Mesh* mesh) {
    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteBuffers(1, &mesh->ibo);
    free(mesh);
}

void Mesh_draw(Mesh* mesh) {

    glBindVertexArray(mesh->vao);

    glDrawElements(GL_TRIANGLES, mesh->size, GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);
}
