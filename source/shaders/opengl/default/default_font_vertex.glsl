#version 460 core

layout( location = 0 ) in vec3 i_position_ts;
layout( location = 1 ) in vec2 i_tex_coord_ts;

out vert_frag
{
	vec2 tex_coord_ts;
}
OUT;

uniform mat4 u_projection_view_matrix;

void main()
{
	gl_Position      = u_projection_view_matrix * vec4( i_position_ts, 1 );
	OUT.tex_coord_ts = i_tex_coord_ts;
}