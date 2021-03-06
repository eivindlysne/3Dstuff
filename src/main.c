#include <stdio.h>
#include <stdbool.h>

// TODO: should be moved out later
#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "bstrlib/bstrlib.h"

#include "hash_map.h"
#include "display.h"
#include "input.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "vertex.h"
#include "transform.h"
#include "camera.h"
#include "level.h"
#include "skybox.h"
#include "text.h"
#include "timer.h"

#define WINDOW_TITLE "Okapi"
#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540

static const unsigned int frame_rate = 60;
static const double frame_time = 1.0 / frame_rate;


Mesh* init_test_mesh() {
    Vertex vertices[] = {
        {.position = {-0.5, -1.0, 0.0f}, .tex_coord = {0.0, 0.0}},
        {.position = { 0.5, -1.0, 0.0f}, .tex_coord = {1.0, 0.0}},
        {.position = { 0.5,  0.0, 0.0f}, .tex_coord = {1.0, 1.0}},
        {.position = {-0.5,  0.0, 0.0f}, .tex_coord = {0.0, 1.0}},
    };
    unsigned short indices[] = {
        0, 2, 3, 1, 2, 0,
    };
    return Mesh_init(
        vertices,
        sizeof(vertices) / sizeof(Vertex),
        indices,
        sizeof(indices) / sizeof(unsigned short)
    );
}

Mesh* init_floor_mesh() {
    unsigned int num_tiles = 256;
    int range = sqrt(num_tiles) / 2;
    float size = 1.0f; float plane = -1.0f;

    Vertex vertices[num_tiles * 4];
    unsigned short indices[num_tiles * 6];

    unsigned int vx = 0, ix = 0;
    for (int i = -range; i < range; i++) {
        for (int j = -range; j < range; j++) {
            vertices[vx] = (Vertex) {.position = {size*i, plane, size*j}, .tex_coord = {0.0, 0.0}};
            vertices[vx + 1] = (Vertex) {.position = {size*i+size, plane, size*j}, .tex_coord = {1.0, 0.0}};
            vertices[vx + 2] = (Vertex) {.position = {size*i+size, plane, size*j+size}, .tex_coord = {1.0, 1.0}};
            vertices[vx + 3] = (Vertex) {.position = {size*i, plane, size*j+size}, .tex_coord = {0.0, 1.0}};
            indices[ix] = vx;
            indices[ix + 1] = vx + 2;
            indices[ix + 2] = vx + 3;
            indices[ix + 3] = vx + 1;
            indices[ix + 4] = vx + 2;
            indices[ix + 5] = vx;
            vx += 4; ix += 6;
        }
    }
    return Mesh_init(
        vertices,
        sizeof(vertices) / sizeof(Vertex),
        indices,
        sizeof(indices) / sizeof(unsigned short)
    );
}

Texture* init_test_texture() {
    return Texture_init(
        bfromcstr("res/textures/crack.png"),
        bfromcstr("res/textures/crack_n.png")
    );
}

Shader* init_basic_shader() {
    Shader* basic_shader = Shader_init("basic_vert.glsl", "basic_frag.glsl");
    Shader_bind(basic_shader);
    Shader_set_transform_location(basic_shader);
    Shader_set_uniform_location(basic_shader, bfromcstr("projection"));
    Shader_set_uniform_location(basic_shader, bfromcstr("diffuse"));
    Shader_set_uniform_location(basic_shader, bfromcstr("normal"));
    Shader_set_uniform_i(basic_shader, bfromcstr("diffuse"), 0);
    Shader_set_uniform_i(basic_shader, bfromcstr("normal"), 1);
    Shader_bind(NULL);
    return basic_shader;
}

Shader* init_text_shader() {
    Shader* shader = Shader_init("text_vert.glsl", "text_frag.glsl");
    Shader_bind(shader);
    Shader_set_uniform_location(shader, bfromcstr("projection"));
    Shader_set_uniform_location(shader, bfromcstr("font"));
    Shader_set_uniform_i(shader, bfromcstr("font"), 0);
    Shader_bind(NULL);
    return shader;
}

void test_stuff() {}


int main(int argc, char const *argv[]) {

    test_stuff();

    Display* display = Display_init(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

    Input* input = Input_init();

    Shader* basic_shader = init_basic_shader();

    Shader* text_shader = init_text_shader();

    Transform transform = Transform_init_default();
    vec3 camera_position = {0.0f, 0.0f, 3.0f};
    Camera* camera = Camera_init(
        camera_position,
        70.0f,
        (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT,
        0.01f,
        1000.0f
    );
    //Camera_look_at(camera, (vec3) {0,0,0}, G_FORWARD); Does not work

    Skybox* skybox = Skybox_init(
        bfromcstr("res/textures/skybox/neg_z.png"),
        bfromcstr("res/textures/skybox/pos_z.png"),
        bfromcstr("res/textures/skybox/pos_y.png"),
        bfromcstr("res/textures/skybox/neg_y.png"),
        bfromcstr("res/textures/skybox/neg_x.png"),
        bfromcstr("res/textures/skybox/pos_x.png")
    );

    Mesh* test_mesh = init_test_mesh();
    Mesh* floor_mesh = init_floor_mesh();

    Texture* test_texture = init_test_texture();

    TextMesh* text_mesh = TextMesh_init();
    TextMesh_add(text_mesh, bfromcstr("FPS: 0"));

    bool running = true;

    double last_time = get_time();
    double frame_counter = 0.0;
    double unprocessed_time = 0.0;
    unsigned int frames = 0;

    while (running) {

        bool render = false;

        double start_time = get_time();
        double elapsed_time = start_time - last_time;
        last_time = start_time;

        unprocessed_time += elapsed_time;
        frame_counter += elapsed_time;

        // Display FPS every second
        if (frame_counter >= 1.0) {

            int buffer_size = 9;
            char buffer[buffer_size];
            snprintf(buffer, buffer_size, "FPS: %u", frames);
            TextMesh_set(text_mesh, bfromcstr(buffer), 0);

            frames = 0;
            frame_counter = 0;
        }

        while (unprocessed_time > frame_time) {

            // Process input
            Input_update(input, display, &running);

            // Update
            Camera_update(camera, input);
            Skybox_update(skybox, camera);
            TextMesh_update(text_mesh);

            render = true;

            unprocessed_time -= frame_time;
        }

        if (render) {
            // Render
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            mat4x4 projection;
            Camera_view_projection(camera, projection);

            Skybox_draw(skybox, projection);

            Shader_bind(basic_shader);
            Shader_set_uniform_mat4x4(basic_shader, bfromcstr("projection"), projection);

            Shader_set_transform(basic_shader, &transform);

            Texture_bind(test_texture);
            Mesh_draw(test_mesh);
            Mesh_draw(floor_mesh);
            Shader_bind(NULL);

            Shader_bind(text_shader);
            mat4x4_identity(projection);
            mat4x4_ortho(
                projection,
                0.0f,
                WINDOW_WIDTH,
                0.0f,
                WINDOW_HEIGHT,
                1.0f,
                -1.0f
            );
            Shader_set_uniform_mat4x4(text_shader, bfromcstr("projection"), projection);
            TextMesh_draw(text_mesh);
            Shader_bind(NULL);

            SDL_GL_SwapWindow(display->window);

            frames++;
        } else {
            // Sleep one ms, not Reliable
            SDL_Delay(1);
            //printf("Slept");
        }

    }

    Display_destroy(display);
    Input_destroy(input);
    Shader_destroy(basic_shader);
    Shader_destroy(text_shader);
    Mesh_destroy(test_mesh);
    Mesh_destroy(floor_mesh);
    TextMesh_destroy(text_mesh);
    Texture_destroy(test_texture);
    Camera_destroy(camera);
    Skybox_destroy(skybox);

    return 0;
}
