#ifndef OKAPI_LEVEL_H
#define OKAPI_LEVEL_H

#include "bstrlib/bstrlib.h"
#include "tmx/tmx.h"

#include "texture.h"
#include "mesh.h"

typedef struct {
    tmx_map* map;
    Texture* tilesheet;
    Mesh* mesh;
} Level;

Level* Level_init(bstring const file_name);

void Level_destroy(Level* level);

void Level_draw(Level* const level);

#endif
