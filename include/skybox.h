#ifndef OKAPI_SKYBOX_H
#define OKAPI_SKYBOX_H

#include <GL/glew.h>

#include "bstrlib/bstrlib.h"
#include "linmath/linmath.h"

#include "shader.h"
#include "camera.h"
#include "transform.h"

typedef struct {
    GLuint vao;
    GLuint vbo;
    GLuint handle;
    Shader* shader;
    Transform transform;
} Skybox;


Skybox* Skybox_init(
    bstring front,
    bstring back,
    bstring top,
    bstring bottom,
    bstring left,
    bstring right
);

void Skybox_destroy(Skybox* skybox);

void Skybox_update(Skybox* const skybox, Camera* const camera);

void Skybox_draw(Skybox* const skybox, mat4x4 projection);

#endif
