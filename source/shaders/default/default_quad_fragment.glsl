#version 460 core

in vert_frag
{
    vec2 tex_coord;
} IN;

layout ( location = 0 ) out vec4 o_color;

uniform vec4 u_color;
        
uniform sampler2D u_texture;

void main( )
{
    o_color = texture( u_texture, IN.tex_coord ) * u_color;
}