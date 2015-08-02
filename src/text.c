#include "stb/stretchy_buffer.h"
#include "stb/stb_image.h"

#include "text.h"
#include "vertex.h"


static void init_font(TextMesh* const text_mesh, bstring file_name) {

    int width, height, num_components;
    unsigned char* data = stbi_load(
        bdata(file_name),
        &width, &height,
        &num_components,
        STBI_rgb_alpha
    );

    if (!data) {
        printf("Failed to load texture: %s\n", bdata(file_name));
        return;
    }

    glGenTextures(1, &text_mesh->font_tex);
    glBindTexture(GL_TEXTURE_2D, text_mesh->font_tex);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
}


TextMesh* TextMesh_init() {

    TextMesh* text_mesh = malloc(sizeof(TextMesh));
    text_mesh->strings = NULL;
    text_mesh->size = 0;

    bstring font_file = bfromcstr("res/fonts/font.png");
    init_font(text_mesh, font_file);
    bdestroy(font_file);

    glGenVertexArrays(1, &text_mesh->vao);
    glBindVertexArray(text_mesh->vao);
    glGenBuffers(1, &text_mesh->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, text_mesh->vbo);

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
    glBindVertexArray(0);

    return text_mesh;
}

void TextMesh_destroy(TextMesh* text_mesh) {
    glDeleteVertexArrays(1, &text_mesh->vao);
    glDeleteBuffers(1, &text_mesh->vbo);
    glDeleteTextures(1, &text_mesh->font_tex);
    if (text_mesh->strings != NULL) {
        for (size_t i = 0; i < sb_count(text_mesh->strings); i++) {
            bdestroy(text_mesh->strings[i]);
        }
        sb_free(text_mesh->strings);
    }
    free(text_mesh);
}

void TextMesh_add(TextMesh* const text_mesh, bstring const text) {

    sb_push(text_mesh->strings, text);
}

void TextMesh_set(
    TextMesh* const text_mesh,
    bstring const text,
    unsigned int index
) {
    bassign(text_mesh->strings[index], text);
    bdestroy(text);
}

void TextMesh_update(TextMesh* const text_mesh) {
    Vertex* vertices = NULL;
    float c_size = 32;
    double step = 1.0 / 16.0;

    unsigned int num_strings = sb_count(text_mesh->strings);
    for (size_t i = 0; i < num_strings; i++) {
        bstring s = text_mesh->strings[i];

        for (size_t j = 0; j < blength(s); j++) {
            char c = bdata(s)[j];
            float uv_x = (c % 16) / 16.0f;
            float uv_y = (c / 16) / 16.0f;
            float x = j * (c_size / 2.0);

            Vertex v1 = {.position = {x,0,0}, .tex_coord = {uv_x, 1.0f - (uv_y + step)}};
            Vertex v2 = {.position = {x+c_size,0,0}, .tex_coord = {uv_x + step, 1.0f - (uv_y + step)}};
            Vertex v3 = {.position = {x+c_size,c_size,0}, .tex_coord = {uv_x + step, 1.0f - uv_y}};
            Vertex v4 = {.position = {x,c_size,0}, .tex_coord = {uv_x, 1.0f - uv_y}};

            sb_push(vertices, v1);
            sb_push(vertices, v2);
            sb_push(vertices, v3);
            sb_push(vertices, v4);

            text_mesh->size += 4;
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, text_mesh->vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(Vertex) * sb_count(vertices),
        vertices,
        GL_DYNAMIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    sb_free(vertices);
}

void TextMesh_draw(TextMesh* const text_mesh) {

    glBindTexture(GL_TEXTURE_2D, text_mesh->font_tex);

    glBindVertexArray(text_mesh->vao);
    glDrawArrays(GL_QUADS, 0, text_mesh->size);
    glBindVertexArray(0);
}

void TextMesh_print(TextMesh* const text_mesh) {

    for (size_t n = 0; n < sb_count(text_mesh->strings); n++) {
        printf("%s\n", bdata(text_mesh->strings[n]));
    }
}
