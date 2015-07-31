#version 130

const vec2 Resolution = vec2(960, 540);
const vec4 LightColor = vec4(1.0, 0.8, 0.6, 1.0);
const vec4 AmbientColor = vec4(0.6, 0.6, 1.0, 0.4);
const vec3 Falloff = vec3(0.4, 0.4, 0.4);

in vec3 LightPos;
uniform sampler2D diffuse;
uniform sampler2D normal;

in vec2 vTexCoord;

out vec4 fragColor;


void main() {
    //RGBA of our diffuse color
    vec4 DiffuseColor = texture2D(diffuse, vTexCoord);

    //RGB of our normal map
    vec3 NormalMap = texture2D(normal, vTexCoord).rgb;

    //The delta position of light
    vec3 LightDir = vec3(LightPos.xy - (gl_FragCoord.xy / Resolution.xy), LightPos.z);

    //Correct for aspect ratio
    LightDir.x *= Resolution.x / Resolution.y;

    //Determine distance (used for attenuation) BEFORE we normalize our LightDir
    float D = length(LightDir);

    //normalize our vectors
    vec3 N = normalize(NormalMap * 2.0 - 1.0);
    vec3 L = normalize(LightDir);

    //Pre-multiply light color with intensity
    //Then perform "N dot L" to determine our diffuse term
    vec3 Diffuse = (LightColor.rgb * LightColor.a) * max(dot(N, L), 0.0);

    //pre-multiply ambient color with intensity
    vec3 Ambient = AmbientColor.rgb * AmbientColor.a;

    //calculate attenuation
    float Attenuation = 1.0 / ( Falloff.x + (Falloff.y*D) + (Falloff.z*D*D) );

    //the calculation which brings it all together
    vec3 Intensity = Ambient + Diffuse * Attenuation;
    vec3 FinalColor = DiffuseColor.rgb * Intensity;
    gl_FragColor = vec4(FinalColor, DiffuseColor.a);
}

// void main() {
//
//     vec4 diffuseColor = texture2D(diffuse, vTexCoord);
//     vec3 normalMap = texture2D(normal, vTexCoord).rgb;
//
//     vec3 lightDir = vec3(
//         lightPos.xy - (gl_FragCoord.xy / resolution.xy),
//         lightPos.z
//     );
//     lightDir.x *= resolution.x / resolution.y;
//
//     float D = length(lightDir);
//     vec3 N = normalize(normalMap * 2.0 - 1.0);
//     vec3 L = normalize(lightDir);
//
//     vec3 diffuseIntensity = (lightColor.rgb * lightColor.a) * max(dot(N, L), 0.0);
//     vec3 ambientIntensity = ambientColor.rgb * ambientColor.a;
//
//     float attenuation = 1.0 / (falloff.x + (falloff.y*D) + (falloff.z*D*D));
//
//     vec3 intensity = ambientIntensity + diffuseIntensity * attenuation;
//     vec3 color = diffuseColor.rgb * intensity;
//
//     fragColor = vec4(color, diffuseColor.a);
// }
