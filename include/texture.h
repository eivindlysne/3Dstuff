#ifndef OKAPI_TEXTURE_H
#define OKAPI_TEXTURE_H

#include <GL/glew.h>

#include "bstrlib/bstrlib.h"

typedef struct {
    GLuint handle;
    GLuint normal_handle;
} Texture;

Texture* Texture_init(bstring file_name, bstring file_name_normal);

void Texture_destroy(Texture* texture);

void Texture_bind(Texture* const texture);

void Texture_bind_unit(Texture* texture, unsigned int unit);

#endif
