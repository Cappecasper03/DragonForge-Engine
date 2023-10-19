#version 460 core

in vec3 color;
in vec2 tex_coord;

out vec4 o_color;

uniform sampler2D u_color_map;

void main( )
{
    o_color = texture( u_color_map, tex_coord ) * vec4( color, 1 );
}