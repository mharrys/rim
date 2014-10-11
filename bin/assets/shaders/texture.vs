#version 130

uniform mat4 mvp;

in vec4 vertex_position;

out vec2 tex_coord;

void main()
{
    tex_coord = vertex_position.xy * 0.5 + 0.5;
    gl_Position = mvp * vertex_position;
}
