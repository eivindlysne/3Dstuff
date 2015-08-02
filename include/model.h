#ifndef OKAPI_MODEL_H
#define OKAPI_MODEL_H

#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"

typedef struct {
    Mesh* mesh;
    Shader* shader;
    Texture* texture;
} Model;

typedef struct {
    Model* model;
    Transform transform;
} ModelInstance;

ModelInstance* Model_init(
    Mesh* const mesh,
    Shader* const shader,
    Texture* const texture
);

void Model_destroy(Model* model);

#endif
