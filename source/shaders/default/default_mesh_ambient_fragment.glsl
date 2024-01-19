#version 460 core

in vert_frag
{
    vec2 tex_coord;
} IN;

layout ( location = 0 ) out vec4 o_color;

uniform vec4 u_color;

uniform sampler2D u_color_texture;

void main( )
{
    const vec4 texture_color = texture( u_color_texture, IN.tex_coord );

    o_color = texture_color * u_color;
}