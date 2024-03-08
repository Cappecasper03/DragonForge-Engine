#version 460 core

layout( location = 0 ) in vert_frag
{
	vec2 tex_coord_ts;
}
IN;

layout( location = 0 ) out vec4 o_color;

layout( binding = 0 ) uniform FRAG
{
	vec4 u_color;
	bool u_use_texture;
};

layout( binding = 1 ) uniform sampler2D u_texture;

void main()
{
	const vec4 texture_color = u_use_texture ? texture( u_texture, IN.tex_coord_ts ) : vec4( 1 );

	o_color = texture_color * u_color;
}