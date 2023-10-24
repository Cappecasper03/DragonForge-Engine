#version 460 core

in vert_frag
{
    vec2 tex_coord;
} IN;

layout ( location = 0 ) out vec4 o_color;

uniform vec4 u_color;
uniform bool u_use_texture;

uniform sampler2D u_texture;

void main( )
{
    const vec4 texture_color = u_use_texture ? texture( u_texture, IN.tex_coord ) : vec4( 1 );

    o_color = texture_color * u_color;
}