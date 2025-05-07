#version 430 core

in vert_frag
{
	vec3 position_ws;
	vec2 tex_coord_ts;
}
IN;

layout( location = 0 ) out vec3 out_position;
layout( location = 1 ) out vec3 out_normal;
layout( location = 2 ) out vec4 out_color_specular;

uniform vec4 u_color;

uniform sampler2D u_texture;

void main()
{
	out_position           = IN.position_ws;
	out_normal             = vec3( 0, 0, 1 );
	out_color_specular.rgb = u_color.rgb * texture( u_texture, IN.tex_coord_ts ).rgb;
	out_color_specular.a   = 0;
}