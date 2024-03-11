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
	int  u_layer;
};

layout( binding = 1 ) uniform sampler2DArray u_texture_array;

void main()
{
	const float alpha = texture( u_texture_array, vec3( IN.tex_coord_ts, u_layer ) ).r;

	o_color = vec4( u_color.rgb, alpha );
}