// #if defined(__linux__)
//     #define OS_LINUX
// #else
//     #define OS_OTHER
// #endif

#include <stdio.h>
#include <stdbool.h>

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
#include "timer.h"

#define WINDOW_TITLE "Okapi"
#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540

static const unsigned int frame_rate = 60;
static const double frame_time = 1.0 / frame_rate;

void init_OpenGL(void) {
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW!");
        return;
    }

    printf("----------------------------------------------------------------\n");
    printf("Graphics Successfully Initialized\n");
    printf("OpenGL Info\n");
    printf("    Version: %s\n", glGetString(GL_VERSION));
    printf("     Vendor: %s\n", glGetString(GL_VENDOR));
    printf("   Renderer: %s\n", glGetString(GL_RENDERER));
    printf("    Shading: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("----------------------------------------------------------------\n");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glViewport(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT);

    glEnable(GL_TEXTURE_2D); // Not needed?
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); // Dunno if doin' anything

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //glEnable(GL_CULL_FACE); // Cannot rotate?
    //glCullFace(GL_BACK);
}

Mesh* init_test_mesh() {
    Vertex vertices[] = {
        {.position = {-0.5, -0.5, 0.0f}, .tex_coord = {0.0, 0.0}},
        {.position = { 0.5, -0.5, 0.0f}, .tex_coord = {1.0, 0.0}},
        {.position = { 0.5,  0.5, 0.0f}, .tex_coord = {1.0, 1.0}},
        {.position = {-0.5,  0.5, 0.0f}, .tex_coord = {0.0, 1.0}},
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

Texture* init_test_texture() {
    return Texture_init(
        bfromcstr("res/textures/crack.png"),
        bfromcstr("res/textures/crack_n.png")
    );
}

void test_stuff() {
    #ifdef OS_LINUX
        printf("LINUUUX\n");
    #endif
}


int main(int argc, char const *argv[]) {

    test_stuff();

    Display* display = Display_init(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
    init_OpenGL();

    Input* input = Input_init();
    Shader* basic_shader = Shader_init("basic_vert.glsl", "basic_frag.glsl");
    Shader_bind(basic_shader);
    Shader_set_transform_location(basic_shader);
    Shader_set_uniform_location(basic_shader, bfromcstr("projection"));
    Shader_set_uniform_location(basic_shader, bfromcstr("diffuse"));
    Shader_set_uniform_location(basic_shader, bfromcstr("normal"));
    Shader_set_uniform_i(basic_shader, bfromcstr("diffuse"), 0);
    Shader_set_uniform_i(basic_shader, bfromcstr("normal"), 1);
    Shader_bind(NULL);

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

    Mesh* test_mesh = init_test_mesh();
    Texture* test_texture = init_test_texture();
    //Level* level = Level_init(NULL);
    Skybox* skybox = Skybox_init(
        bfromcstr("res/textures/skybox/neg_z.png"),
        bfromcstr("res/textures/skybox/pos_z.png"),
        bfromcstr("res/textures/skybox/pos_y.png"),
        bfromcstr("res/textures/skybox/neg_y.png"),
        bfromcstr("res/textures/skybox/neg_x.png"),
        bfromcstr("res/textures/skybox/pos_x.png")
    );

    bool running = true;
    printf("GETTING time\n");
    double last_time = get_time();
    printf("GOT time\n");
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

            printf("FPS: %u\n", frames);

            frames = 0;
            frame_counter = 0;
        }

        while (unprocessed_time > frame_time) {

            // Process input
            Input_update(input, display, &running);

            // Update
            Camera_update(camera, input);
            Skybox_update(skybox, camera);

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
    Mesh_destroy(test_mesh);
    Texture_destroy(test_texture);
    Camera_destroy(camera);
    //Level_destroy(level);
    Skybox_destroy(skybox);

    return 0;
}
