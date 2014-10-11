#version 130

// material ambient, diffuse, specular and shininess
uniform vec3 Ka = vec3(0.2);
uniform vec3 Kd = vec3(0.2, 0.8, 0.2);
uniform vec3 Ks = vec3(0.3);
uniform float shininess = 50.0;

// light source ambient, diffuse and specular
uniform vec3 La = vec3(0.2);
uniform vec3 Ld = vec3(1.0);
uniform vec3 Ls = vec3(1.0);

uniform vec4 light_position;

uniform float rim_width = 0.8;

in vec4 position;
in vec3 normal;

out vec4 frag_color;

vec3 blinn_phong_model()
{
    // direction from fragment to light source
    vec3 s = normalize(light_position.xyz - position.xyz);
    // direction vector pointing towards the viewer
    vec3 v = normalize(-position.xyz);
    // halfway vector between the viewer and light source
    vec3 h = normalize(v + s);

    vec3 n = normalize(normal);
    vec3 ambient = La * Ka;
    vec3 diffuse = Ld * Kd * max(dot(s, n), 0.0);
    vec3 specular = max(Ls * Ks * pow(max(dot(n, h), 0.0), 4.0 * shininess), 0.0);

    return ambient + diffuse + specular;
}

void main()
{
    float dist = distance(light_position.xyz, position.xyz);
    float attenuation = 1.0 / (1.0 + (0.5 * dist + 0.03 * dist * dist));

    frag_color = vec4(blinn_phong_model() * attenuation, 1.0);
}
