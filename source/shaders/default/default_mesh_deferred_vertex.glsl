#version 460 core

layout ( location = 0 ) in vec3 i_position_ts;
layout ( location = 1 ) in vec3 i_normal_ts;
layout ( location = 2 ) in vec3 i_tangent_ts;
layout ( location = 3 ) in vec3 i_bitangent_ts;
layout ( location = 4 ) in vec2 i_tex_coord_ts;

out vert_frag
{
    vec3 position_ws;
    vec2 tex_coord_ts;
    mat3 tbn_ws;
} OUT;

uniform mat4 u_world_matrix;
uniform mat4 u_projection_view_matrix;

void main( )
{
    const vec3 position_ws = vec4( u_world_matrix * vec4( i_position_ts, 1 ) ).rgb;

    gl_Position = u_projection_view_matrix * u_world_matrix * vec4( i_position_ts, 1 );
    OUT.position_ws = position_ws;
    OUT.tex_coord_ts = i_tex_coord_ts;
    OUT.tbn_ws = mat3( i_tangent_ts, i_bitangent_ts, i_normal_ts );
}