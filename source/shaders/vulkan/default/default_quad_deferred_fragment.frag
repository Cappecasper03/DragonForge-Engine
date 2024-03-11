#version 460 core

layout( location = 0 ) in vert_frag
{
	vec2 tex_coord_ts;
}
IN;

layout( location = 0 ) out vec4 o_color;

layout( binding = 0 ) uniform sampler2D u_position_texture;
layout( binding = 1 ) uniform sampler2D u_normal_texture;
layout( binding = 2 ) uniform sampler2D u_color_specular_texture;

void main()
{
	const vec4  position       = texture( u_position_texture, IN.tex_coord_ts );
	const vec4  normal         = texture( u_normal_texture, IN.tex_coord_ts ) * 2 - 1;
	const vec4  color_specular = texture( u_color_specular_texture, IN.tex_coord_ts );
	const vec3  color          = color_specular.rgb;
	const float specular       = color_specular.a;

	o_color = position;
}