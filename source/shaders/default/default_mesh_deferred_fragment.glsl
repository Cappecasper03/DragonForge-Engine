#version 460 core

in vert_frag
{
    vec3 position_ws;
    vec2 tex_coord_ts;
    mat3 tbn_ws;
} IN;

layout ( location = 0 ) out vec3 o_position_ws;
layout ( location = 1 ) out vec3 o_normal_ws;
layout ( location = 2 ) out vec4 o_color_specular_ts;

uniform sampler2D u_color_texture;
uniform sampler2D u_normal_texture;
uniform sampler2D u_specular_texture;

void main( )
{
    const vec3 normal_map_ts = texture( u_normal_texture, IN.tex_coord_ts ).xyz * 2 - 1;
    const vec3 normal_ws = ( normalize( IN.tbn_ws * normal_map_ts ) + 1 ) / 2;

    o_position_ws = IN.position_ws;
    o_normal_ws = normal_ws;
    o_color_specular_ts.rgb = texture( u_color_texture, IN.tex_coord_ts ).rgb;
    o_color_specular_ts.a = texture( u_specular_texture, IN.tex_coord_ts ).r;
}