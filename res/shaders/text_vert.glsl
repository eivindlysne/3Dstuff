#version 130

in vec3 position;
in vec2 tex_coord;

out vec2 vTexCoord;

void main() {
    vTexCoord = tex_coord;
    gl_Position = vec4(position, 1.0);
}
