#include <stdlib.h>
#include <stdio.h>

#include "stb/stretchy_buffer.h"

#include "level.h"
#include "vertex.h"

// static void print_map_info(tmx_map* const map) {
//
//     printf("----------------------------------------------------------------\n");
//     printf("Map width (tiles):\t%d\n", map->width);
//     printf("Map height (tiles):\t%d\n", map->height);
//     printf("Tile size (pixels):\t%dx%d\n", map->tile_width, map->tile_height);
//     printf(
//         "Map size (pixels)\t%dx%d\n",
//         map->width * map->tile_width,
//         map->height * map->tile_height
//     );
// }

static Mesh* generate_mesh(tmx_map* const map) {

    Mesh* mesh;
    tmx_layer* layers = map->ly_head;
    Vertex* vertices = NULL;
    unsigned short* indices = NULL;

    while (layers) {
        if (layers->visible) {
            switch (layers->type) {
                case L_OBJGR:
                    printf("Found object layer\n");
                    break;
                case L_IMAGE:
                    printf("Found image layer\n");
                    break;
                case L_LAYER:
                    printf("Found tile layer\n");

                    unsigned int tile_count = 0;
                    unsigned int vertex_counter = 0;
                    unsigned int index_counter = 0;

                    for (size_t i = 0; i < map->height; i++) {
                        for (size_t j = 0; j < map->width; j++) {
                            unsigned int gid = layers->content.gids[(i*map->width)+j];
                            if (gid <= 0) {
                                continue;
                            }

                            tile_count++;

                            // TODO: Not working!
                            float x = -1.0 + (j * map->tile_width)/(960.0/32);
                            float y = -1.0 + (i * map->tile_height)/(540.0/32);
                            float z = 0.0;
                            float w = (map->tile_width)/(960.0/32);
                            float h = (map->tile_height)/(540.0/32);
                            Vertex v1 = {
                                .position = {x,y,z}, .tex_coord = {0.0, 0.0}
                            };
                            Vertex v2 = {
                                .position = {x+w,y,z}, .tex_coord = {1.0, 0.0}
                            };
                            Vertex v3 = {
                                .position = {x+w,y+h,z}, .tex_coord = {1.0, 1.0}
                            };
                            Vertex v4 = {
                                .position = {x,y+h,z}, .tex_coord = {0.0, 1.0}
                            };
                            printf("%f %f\n", v1.position[0], v1.position[1]);
                            // TODO: Change to use sb_add()
                            sb_push(vertices, v1);
                            sb_push(vertices, v2);
                            sb_push(vertices, v3);
                            sb_push(vertices, v4);
                            sb_push(indices, vertex_counter);
                            sb_push(indices, vertex_counter + 2);
                            sb_push(indices, vertex_counter + 3);
                            sb_push(indices, vertex_counter + 1);
                            sb_push(indices, vertex_counter + 2);
                            sb_push(indices, vertex_counter);

                            vertex_counter += 4;
                            index_counter += 6;
                        }
                    }

                    printf("%u\n", sb_count(vertices));
                    printf("%u\n", sb_count(indices));
                    printf("%u\n", vertex_counter);
                    printf("%u\n", index_counter);
                    mesh = Mesh_init(
                        vertices,
                        vertex_counter,
                        indices,
                        index_counter
                    );
                    sb_free(vertices);
                    sb_free(indices);

                    break;
                case L_NONE:
                    printf("Found NONE layer\n");
                    break;
            }
        }
        layers = layers->next;
    }

    return mesh;
}


Level* Level_init(bstring const file_name) {

    Level* level = malloc(sizeof(Level));

    level->map = tmx_load("res/levels/level_1.tmx");
    if (!level->map) {
        tmx_perror("tmx_load");
        return NULL;
    }

    level->mesh = generate_mesh(level->map);

    // TODO: Temp!!!
    level->tilesheet = Texture_init(
        bfromcstr("res/textures/crack.png"),
        bfromcstr("res/textures/crack_n.png")
    );

    return level;
}

void Level_destroy(Level* level) {

    tmx_map_free(level->map);
    Texture_destroy(level->tilesheet);
    Mesh_destroy(level->mesh);
    free(level);
}

void Level_draw(Level* const level) {
    Texture_bind(level->tilesheet);
    Mesh_draw(level->mesh);
}
