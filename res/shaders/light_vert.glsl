#version 130

uniform mat4 mvp;
uniform vec3 uLightPos;

in vec3 position;
in vec2 tex_coord;

out vec2 vTexCoord;
out vec3 LightPos;

void main() {
    vTexCoord = tex_coord;
    LightPos = (mvp * vec4(uLightPos, 0.0)).xyz;
    gl_Position = mvp * vec4(position, 1.0);
}
