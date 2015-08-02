#ifndef OKAPI_TEXT_H
#define OKAPI_TEXT_H

#include <stdio.h>

#include <GL/glew.h>

#include "bstrlib/bstrlib.h"

// TODO: Use this instead of bstring for additional data
typedef struct {
    bstring string;
    float x, y;
} TextElement;

typedef struct {
    GLuint vao;
    GLuint vbo;
    GLuint font_tex;
    bstring* strings;
    unsigned int size;
} TextMesh;

TextMesh* TextMesh_init();

void TextMesh_destroy(TextMesh* text_mesh);

void TextMesh_add(TextMesh* const text_mesh, bstring const text);

void TextMesh_set(
    TextMesh* const text_mesh,
    bstring const text,
    unsigned int index
);

void TextMesh_update(TextMesh* const text_mesh);

void TextMesh_draw(TextMesh* const text_mesh);

void TextMesh_print(TextMesh* const text_mesh);

#endif
