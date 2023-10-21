#version 460 core

in vert_frag
{
    vec2 tex_coord;
} IN;

layout ( location = 0 ) out vec4 o_color;

uniform sampler2D u_color_map;

void main( )
{
    o_color = texture( u_color_map, IN.tex_coord );
}