#version 130

struct Transform {
    vec3 position;
    vec4 orientation;
    vec3 scale;
};

vec3 quaternion_rotate(vec4 q, vec3 v) {
    return v + 2.0 * cross(cross(v, q.xyz) + q.w * v, q.xyz);
}

uniform mat4 projection;
uniform Transform transform;

in vec3 position;
in vec2 tex_coord;

out vec2 vTexCoord;

void main() {
    vTexCoord = tex_coord;

    vec3 pos = position;
    pos = transform.position + quaternion_rotate(
        transform.orientation,
        transform.scale * pos
    );

    gl_Position = projection * vec4(pos, 1.0);
}
