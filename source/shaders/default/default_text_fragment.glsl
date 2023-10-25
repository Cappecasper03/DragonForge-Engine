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
    const float alpha = texture( u_texture, IN.tex_coord ).r;

    o_color = vec4( u_color.rgb, alpha );
}