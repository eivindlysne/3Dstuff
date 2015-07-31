#ifndef OKAPI_SHADER_H
#define OKAPI_SHADER_H

#include <GL/glew.h>

#include "bstrlib/bstrlib.h"
#include "linmath/linmath.h"

#include "transform.h"
#include "hash_map.h"

#define SHADER_DIR "./res/shaders/"


typedef struct {
    GLuint program;
    GLuint vertex_shader;
    GLuint fragment_shader;
    HashMap* uniform_location;
} Shader;


Shader* Shader_init(char* vertex_shader_filename, char* fragment_shader_filename);

void Shader_bind(Shader* shader);

void Shader_destroy(Shader* shader);

void Shader_set_uniform_location(Shader* shader, bstring name);

void Shader_set_transform_location(Shader* const shader);

void Shader_set_uniform_i(Shader* shader, bstring name, unsigned int i);

void Shader_set_uniform_vec2(Shader* shader, bstring name, vec2 vec);

void Shader_set_uniform_vec3(Shader* shader, bstring name, vec3 vec);

void Shader_set_uniform_quat(Shader* const shader, bstring name, quat q);

void Shader_set_uniform_mat4x4(Shader* shader, bstring name, mat4x4 mat);

void Shader_set_uniform_float(Shader* shader, bstring name, float f);

void Shader_set_transform(Shader* const shader, Transform* const transform);

#endif
