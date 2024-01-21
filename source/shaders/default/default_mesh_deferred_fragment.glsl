#version 460 core

in vert_frag
{
    vec3 position;
    vec2 tex_coord;
    mat3 tbn;
} IN;

layout ( location = 0 ) out vec3 o_position;
layout ( location = 1 ) out vec3 o_normal;
layout ( location = 2 ) out vec4 o_color_specular;

uniform sampler2D u_color_texture;
uniform sampler2D u_normal_texture;
uniform sampler2D u_specular_texture;

void main( )
{
    const vec3 normal_map = texture( u_normal_texture, IN.tex_coord ).xyz * 2 - 1;
    const vec3 normal = normalize( IN.tbn * normal_map ) - 1 / 2;

    o_position = IN.position;
    o_normal = normal;
    o_color_specular.rgb = texture( u_color_texture, IN.tex_coord ).rgb;
    o_color_specular.a = texture( u_specular_texture, IN.tex_coord ).r;
}