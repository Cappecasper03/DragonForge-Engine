#version 460 core
#extension GL_EXT_buffer_reference : require

struct sVertex
{
	vec3 position;
	vec2 tex_coord;
};

layout( buffer_reference, std430 ) readonly buffer sVertexBuffer
{
	sVertex vertices[];
};

layout( push_constant ) uniform constants
{
	mat4          u_world_matrix;
	sVertexBuffer vertex_buffer;
}
IN;

layout( location = 0 ) out vert_frag
{
	vec2 tex_coord_ts;
}
OUT;

void main()
{
	sVertex vertex = IN.vertex_buffer.vertices[ gl_VertexIndex ];

	gl_Position      = IN.u_view_projection_matrix * IN.u_world_matrix * vec4( vertex.position, 1 );
	OUT.tex_coord_ts = vertex.tex_coord;
}