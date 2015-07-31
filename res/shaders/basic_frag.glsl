#version 130

uniform sampler2D diffuse;
uniform sampler2D normal;

in vec2 vTexCoord;

out vec4 fragColor;

void main() {
    fragColor = texture2D(diffuse, vTexCoord);
}
