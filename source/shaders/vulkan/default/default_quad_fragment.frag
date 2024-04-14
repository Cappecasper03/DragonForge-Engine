#version 460 core

layout( location = 0 ) in vert_frag
{
	vec2 tex_coord_ts;
}
IN;

layout( location = 0 ) out vec4 o_color;

layout( set = 1, binding = 0 ) uniform sFragmentUniforms
{
	vec4 color;
}
IN_FRAGMENT;

layout( set = 1, binding = 1 ) uniform sampler2D u_texture;

void main()
{
	const vec4 texture_color = IN_FRAGMENT.color.a > 0 ? texture( u_texture, IN.tex_coord_ts ) : vec4( 1 );

	o_color = texture_color * vec4( IN_FRAGMENT.color.rgb, 1 );
}