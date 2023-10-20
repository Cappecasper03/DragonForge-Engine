#version 460 core

uniform sampler2D u_color_map;

in vec2 tex_coord;

layout ( location = 0 ) out vec4 o_color;

void main( )
{
    o_color = texture( u_color_map, tex_coord );
}