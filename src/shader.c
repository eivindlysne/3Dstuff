#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "shader.h"

static bstring read_file(bstring path) {

    int buffer_size = 512;
    char buffer[buffer_size];
    FILE* file = fopen(bdata(path), "r");

    if (!file) {
        printf("Failed to open file:  %s\n", bdata(path));
        return NULL;
    }

    bstring contents = NULL;
    while (fgets(buffer, buffer_size, file)) {
        if (contents == NULL) {
            contents = bfromcstr(buffer);
        } else {
            bconcat(contents, bfromcstr(buffer));
        }
    }

    fclose(file);

    return contents;
}

static void check_shader_error(
        GLuint shader,
        GLuint flag,
        bool is_program,
        char* msg) {
    GLint success = 0;
    GLchar error[1024] = {0};

    if (is_program)
        glGetProgramiv(shader, flag, &success);
    else
        glGetShaderiv(shader, flag, &success);

    if (success == GL_FALSE) {
        if (is_program)
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        else
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);

        printf("%s: %s\n", msg, error);
    }
}

static void compile_shader(bstring src, GLuint shader) {

    const GLchar* s[1];
    GLint s_length[1];
    s[0] = bdata(src);
    s_length[0] = blength(src);

    glShaderSource(shader, 1, s, s_length);
    glCompileShader(shader);

    check_shader_error(shader, GL_COMPILE_STATUS, false, "Shader compilation: ");
}

Shader* Shader_init(
        char* vertex_shader_filename,
        char* fragment_shader_filename) {

    bstring vertex_shader_path = bfromcstr(SHADER_DIR);
    bconcat(vertex_shader_path, bfromcstr(vertex_shader_filename));

    bstring fragment_shader_path = bfromcstr(SHADER_DIR);
    bconcat(fragment_shader_path, bfromcstr(fragment_shader_filename));

    bstring vertex_shader_src = read_file(vertex_shader_path);
    bstring fragment_shader_src = read_file(fragment_shader_path);

    Shader* shader = malloc(sizeof(Shader));
    shader->program = glCreateProgram();
    shader->vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    shader->fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    shader->uniform_location = HashMap_init();

    compile_shader(vertex_shader_src, shader->vertex_shader);
    compile_shader(fragment_shader_src, shader->fragment_shader);

    glAttachShader(shader->program, shader->vertex_shader);
    glAttachShader(shader->program, shader->fragment_shader);
    glLinkProgram(shader->program);
    glValidateProgram(shader->program);
    check_shader_error(shader->program, GL_LINK_STATUS, true, "Shader linking: ");
    check_shader_error(shader->program, GL_VALIDATE_STATUS, true, "Shader validation: ");

    bdestroy(vertex_shader_path);
    bdestroy(fragment_shader_path);
    bdestroy(vertex_shader_src);
    bdestroy(fragment_shader_src);

    return shader;
}

void Shader_bind(Shader* shader) {
    if (shader != NULL) {
        glUseProgram(shader->program);
    } else {
        glUseProgram(0);
    }
}

void Shader_destroy(Shader* shader) {

    glDetachShader(shader->program, shader->vertex_shader);
    glDetachShader(shader->program, shader->fragment_shader);

    glDeleteShader(shader->vertex_shader);
    glDeleteShader(shader->fragment_shader);
    glDeleteProgram(shader->program);

    HashMap_destroy(shader->uniform_location);

    free(shader);
}

void Shader_set_uniform_location(Shader* shader, bstring name) {

    GLint location = glGetUniformLocation(shader->program, bdata(name));
    if (location < 0) {
        printf("Failed to locate uniform: %s\n", bdata(name));
        return;
    }
    HashMap_put(shader->uniform_location, name, location);
}

void Shader_set_transform_location(Shader* const shader) {

    bstring position_name = bfromcstr("transform.position");
    bstring orientation_name = bfromcstr("transform.orientation");
    bstring scale_name = bfromcstr("transform.scale");
    GLint position_location = glGetUniformLocation(
        shader->program,
        bdata(position_name)
    );
    GLint orientation_location = glGetUniformLocation(
        shader->program,
        bdata(orientation_name)
    );
    GLint scale_location = glGetUniformLocation(
        shader->program,
        bdata(scale_name)
    );
    if (position_location < 0 || orientation_location < 0 || scale_location < 0) {
        printf("Failed to locate transform!\n");
        return;
    }
    HashMap_put(shader->uniform_location, position_name, position_location);
    HashMap_put(shader->uniform_location, orientation_name, orientation_location);
    HashMap_put(shader->uniform_location, scale_name, scale_location);
}

void Shader_set_uniform_i(Shader* shader, bstring name, unsigned int i) {

    GLint location = HashMap_get(
        shader->uniform_location,
        name
    );
    glUseProgram(shader->program);
    glUniform1i(location, i);
}

void Shader_set_uniform_vec2(Shader* shader, bstring name, vec2 vec) {

    GLint location = HashMap_get(
        shader->uniform_location,
        name
    );
    glUseProgram(shader->program);
    glUniform2f(location, vec[0], vec[1]);
}

void Shader_set_uniform_vec3(Shader* shader, bstring name, vec3 vec) {

    GLint location = HashMap_get(
        shader->uniform_location,
        name
    );
    glUseProgram(shader->program);
    glUniform3f(location, vec[0], vec[1], vec[2]);
}

void Shader_set_uniform_quat(Shader* const shader, bstring name, quat q) {

    GLint location = HashMap_get(
        shader->uniform_location,
        name
    );
    glUseProgram(shader->program);
    glUniform4f(location, q[0], q[1], q[2], q[3]);
}

void Shader_set_uniform_mat4x4(Shader* shader, bstring name, mat4x4 mat) {

    GLint location = HashMap_get(
        shader->uniform_location,
        name
    );
    glUseProgram(shader->program);
    glUniformMatrix4fv(location, 1, GL_FALSE, (GLfloat*) mat);
}

void Shader_set_uniform_float(Shader* shader, bstring name, float f) {

    GLint location = HashMap_get(
        shader->uniform_location,
        name
    );
    glUseProgram(shader->program);
    glUniform1f(location, f);
}

void Shader_set_transform(Shader* const shader, Transform* const transform) {
    Shader_set_uniform_vec3(
        shader,
        bfromcstr("transform.position"),
        transform->position
    );
    Shader_set_uniform_quat(
        shader,
        bfromcstr("transform.orientation"),
        transform->orientation
    );
    Shader_set_uniform_vec3(
        shader,
        bfromcstr("transform.scale"),
        transform->scale
    );
}
