#version 460 core

layout ( location = 0 ) in vec3 i_position_ts;
layout ( location = 1 ) in vec3 i_normal_ts;
layout ( location = 2 ) in vec3 i_tangent_ts;
layout ( location = 3 ) in vec3 i_bitangent_ts;
layout ( location = 4 ) in vec2 i_tex_coord_ts;

layout ( location = 0 ) out vert_frag
{
    vec2 tex_coord_ts;
} OUT;

layout ( binding = 0 ) uniform VERT {
    mat4 u_world_matrix;
    mat4 u_projection_view_matrix;
};

void main( )
{
    gl_Position = u_projection_view_matrix * u_world_matrix * vec4( i_position_ts, 1 );
    OUT.tex_coord_ts = i_tex_coord_ts;
}