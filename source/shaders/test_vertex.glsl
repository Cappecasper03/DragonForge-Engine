#version 460 core

layout ( location = 0 ) in vec3 i_position;
layout ( location = 1 ) in vec2 i_tex_coord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 tex_coord;

void main( )
{
    gl_Position = u_projection * u_view * u_model * vec4( i_position, 1 );
    tex_coord = i_tex_coord;
}