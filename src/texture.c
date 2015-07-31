#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "texture.h"


static bool flipped = false;

static int load_texture(char* file_name, GLuint* handle) {

    if (!flipped) {
        stbi_set_flip_vertically_on_load(1);
        flipped = true;
    }

    int width, height, num_components;
    unsigned char* data = stbi_load(
        file_name,
        &width, &height,
        &num_components,
        STBI_rgb_alpha
    );

    if (data == NULL) {
        printf("Failed to load texture: %s\n", file_name);
        return -1;
    }

    glGenTextures(1, handle);
    glBindTexture(GL_TEXTURE_2D, *handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );
    stbi_image_free(data);
    return 0;
}


Texture* Texture_init(bstring file_name, bstring file_name_normal) {

    Texture* texture = malloc(sizeof(Texture));

    if (load_texture(bdata(file_name), &texture->handle) != 0 ||
        load_texture(bdata(file_name_normal), &texture->normal_handle) != 0) {

        Texture_destroy(texture);
        return NULL;
    }

    return texture;
}

void Texture_destroy(Texture* texture) {
    glDeleteTextures(1, &texture->handle);
    glDeleteTextures(1, &texture->normal_handle);
    free(texture);
}

void Texture_bind(Texture* const texture) {

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture->normal_handle);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->handle);
}

void Texture_bind_unit(Texture* texture, unsigned int unit) {
    assert(unit >= 0 && unit <= 31);

    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture->handle);
}
