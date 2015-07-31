#version 130

uniform samplerCube skybox;

in vec3 vTexCoord;

out vec4 fragColor;

void main() {
    fragColor = textureCube(skybox, vTexCoord);
}
