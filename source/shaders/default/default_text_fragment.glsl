#version 460 core

in vert_frag
{
    vec2 tex_coord;
} IN;

layout ( location = 0 ) out vec4 o_color;

uniform vec4 u_color;
uniform int u_layer;

uniform sampler2DArray u_texture_array;

void main( )
{
    const float alpha = texture( u_texture_array, vec3( IN.tex_coord, u_layer ) ).r;

    o_color = vec4( u_color.rgb, alpha );
}