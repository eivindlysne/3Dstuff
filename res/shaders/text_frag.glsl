#version 130

uniform sampler2D font;

in vec2 vTexCoord;

out vec4 fragColor;

void main() {
    fragColor = texture2D(font, vTexCoord);
}
