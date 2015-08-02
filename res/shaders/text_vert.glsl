#version 130

uniform mat4 projection;

in vec3 position;
in vec2 tex_coord;

out vec2 vTexCoord;

void main() {
    vTexCoord = tex_coord;
    gl_Position = projection * vec4(position, 1.0);
}
