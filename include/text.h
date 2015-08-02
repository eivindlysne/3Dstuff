#ifndef OKAPI_TEXT_H
#define OKAPI_TEXT_H

#include <stdio.h>

#include <GL/glew.h>

#include "bstrlib/bstrlib.h"


typedef struct {
    GLuint vao;
    GLuint vbo;
    GLuint font_tex;
    bstring* strings;
} TextMesh;

TextMesh* TextMesh_init();

void TextMesh_destroy(TextMesh* text_mesh);

void TextMesh_add(TextMesh* const text_mesh, bstring const text);

void TextMesh_update(TextMesh* const text_mesh);

void TextMesh_draw(TextMesh* const text_mesh);

void TextMesh_print(TextMesh* const text_mesh);

#endif
