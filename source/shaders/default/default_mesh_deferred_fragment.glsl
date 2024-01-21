#version 460 core

in vert_frag
{
    vec3 position;
    vec3 normal;
    vec2 tex_coord;
} IN;

layout ( location = 0 ) out vec3 o_position;
layout ( location = 1 ) out vec3 o_normal;
layout ( location = 2 ) out vec4 o_color_specular;

uniform sampler2D u_color_texture;
uniform sampler2D u_specular_texture;

void main( )
{
    o_position = IN.position;
    o_normal = IN.normal;
    o_color_specular.rgb = texture( u_color_texture, IN.tex_coord ).rgb;
    o_color_specular.a = texture( u_specular_texture, IN.tex_coord ).r;
}