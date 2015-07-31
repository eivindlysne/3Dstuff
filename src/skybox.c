#include <stdlib.h>

#include "stb/stb_image.h"

#include "skybox.h"
#include "vertex.h"


static void load_skybox_side(bstring file_name, GLenum target) {

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

    glTexImage2D(
        target,
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

static void create_skybox_buffer(GLuint* const skybox_vbo) {

    float PT = 10.0f;

    SkyboxVertex vertices[] = {

        // Front
        {.position = {-PT, -PT, PT}},
        {.position = {-PT,  PT, PT}},
        {.position = { PT,  PT, PT}},
        {.position = { PT,  PT, PT}},
        {.position = { PT, -PT, PT}},
        {.position = {-PT, -PT, PT}},

        {.position = { PT, PT, -PT}},
        {.position = {-PT, PT, -PT}},
        {.position = {-PT, -PT, -PT}},
        {.position = {-PT, -PT, -PT}},
        {.position = { PT, -PT, -PT}},
        {.position = { PT, PT, -PT}},

        // Other
        {.position = {-PT, PT, -PT}},
        {.position = { PT, PT, -PT}},
        {.position = { PT, PT, PT}},
        {.position = { PT, PT, PT}},
        {.position = {-PT, PT, PT}},
        {.position = {-PT, PT, -PT}},

        {.position = { PT, -PT, PT}},
        {.position = { PT, -PT, -PT}},
        {.position = {-PT, -PT, -PT}},
        {.position = {-PT, -PT, -PT}},
        {.position = {-PT, -PT, PT}},
        {.position = { PT, -PT, PT}},

        {.position = {-PT, PT, -PT}},
        {.position = {-PT, PT, PT}},
        {.position = {-PT, -PT, PT}},
        {.position = {-PT, -PT, PT}},
        {.position = {-PT, -PT, -PT}},
        {.position = {-PT, PT, -PT}},

        {.position = { PT, -PT, -PT}},
        {.position = { PT, -PT, PT}},
        {.position = { PT, PT, PT}},
        {.position = { PT, PT, PT}},
        {.position = { PT, PT, -PT}},
        {.position = { PT, -PT, -PT}},
    };

    glGenBuffers(1, skybox_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, *skybox_vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        36 * sizeof(SkyboxVertex),
        vertices,
        GL_STATIC_DRAW
    );
}


Skybox* Skybox_init(
    bstring front,
    bstring back,
    bstring top,
    bstring bottom,
    bstring left,
    bstring right
) {

    Skybox* skybox = malloc(sizeof(Skybox));

    glGenTextures(1, &skybox->handle);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->handle);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    load_skybox_side(front, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
    load_skybox_side(back, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
    load_skybox_side(top, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
    load_skybox_side(bottom, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
    load_skybox_side(left, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
    load_skybox_side(right, GL_TEXTURE_CUBE_MAP_POSITIVE_X);

    bdestroy(front);
    bdestroy(back);
    bdestroy(top);
    bdestroy(bottom);
    bdestroy(left);
    bdestroy(right);

    glGenVertexArrays(1, &skybox->vao);
    glBindVertexArray(skybox->vao);

    create_skybox_buffer(&skybox->vbo);

    // Note buffer is bound from create_skybox_buffer()
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    skybox->shader = Shader_init("skybox_vert.glsl", "skybox_frag.glsl");
    Shader_bind(skybox->shader);
    Shader_set_transform_location(skybox->shader);
    Shader_set_uniform_location(skybox->shader, bfromcstr("projection"));
    Shader_set_uniform_location(skybox->shader, bfromcstr("skybox"));
    Shader_set_uniform_i(skybox->shader, bfromcstr("skybox"), 0);
    Shader_bind(NULL);

    skybox->transform = Transform_init_default();

    return skybox;
}

void Skybox_destroy(Skybox* skybox) {
    glDeleteVertexArrays(1, &skybox->vao);
    glDeleteBuffers(1, &skybox->vbo);
    glDeleteTextures(1, &skybox->handle);
    Shader_destroy(skybox->shader);
    free(skybox);
}

void Skybox_update(Skybox* const skybox, Camera* const camera) {
    skybox->transform.position[0] = camera->transform.position[0];
    skybox->transform.position[2] = camera->transform.position[2];
}

void Skybox_draw(Skybox* const skybox, mat4x4 projection) {

    // mat4x4 model;
    // Transform_model_matrix(&skybox->transform, model);
    //
    // mat4x4 mvp;
    // mat4x4_mul(mvp, projection, model);

    glDepthMask(GL_FALSE);

    Shader_bind(skybox->shader);
    Shader_set_transform(skybox->shader, &skybox->transform);
    Shader_set_uniform_mat4x4(skybox->shader, bfromcstr("projection"), projection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->handle);

    glBindVertexArray(skybox->vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    Shader_bind(NULL);
    glDepthMask(GL_TRUE);
}
