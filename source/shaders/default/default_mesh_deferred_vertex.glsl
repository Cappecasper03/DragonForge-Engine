﻿#version 460 core

layout ( location = 0 ) in vec3 i_position;
layout ( location = 1 ) in vec3 i_normal;
layout ( location = 2 ) in vec3 i_tangent;
layout ( location = 3 ) in vec3 i_bitangent;
layout ( location = 4 ) in vec2 i_tex_coord;

out vert_frag
{
    vec3 position;
    vec2 tex_coord;
    mat3 tbn;
} OUT;

uniform mat4 u_model;
uniform mat4 u_projection_view;

void main( )
{
    gl_Position = u_projection_view * u_model * vec4( i_position, 1 );
    OUT.position = i_position;
    OUT.tex_coord = i_tex_coord;
    OUT.tbn = mat3( i_tangent, i_bitangent, i_normal );
}