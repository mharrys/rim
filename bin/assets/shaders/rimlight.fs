#version 130

struct Rim {
    vec3 color;
    float width;
    float alpha;
};

struct Material {
    vec3 diffuse;
    Rim rim;
};

uniform Material material;

in vec4 position;
in vec3 normal;

out vec4 frag_color;

vec4 rim_light(Rim rim)
{
    vec3 n = normalize(normal);
    vec3 v = normalize(-position.xyz);

    float intensity = rim.width - max(dot(v, n), 0.0);
    intensity = max(0.0, intensity); // ignore rim light if negative

    return vec4(intensity * rim.color, rim.alpha);
}

void main()
{
    frag_color = vec4(material.diffuse, 1.0);
    frag_color += rim_light(material.rim);
}
