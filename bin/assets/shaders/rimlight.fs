#version 130

uniform vec3 rim_color = vec3(0.6, 0.6, 0.4);
uniform float rim_width = 0.8;
uniform float alpha = 1.0;

in vec4 position;
in vec3 normal;

out vec4 frag_color;

void main()
{
    vec3 n = normal;
    vec3 v = normalize(-position.xyz);
    float rim_intensity = rim_width - max(dot(v, n), 0.0);

    vec3 rim_light = vec3(smoothstep(0.3, 1.0, rim_intensity)) * rim_color;

    frag_color = vec4(rim_light, alpha);
}
