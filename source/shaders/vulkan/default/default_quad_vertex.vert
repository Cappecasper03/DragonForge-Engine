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
	mat4          world_matrix;
	sVertexBuffer vertex_buffer;
}
IN_VERTEX;

layout( set = 0, binding = 0 ) uniform sVertexSceneConstants
{
	mat4 view;
	mat4 projection;
	mat4 view_projection;
}
IN_SCENE;

layout( location = 0 ) out vert_frag
{
	vec2 tex_coord_ts;
}
OUT;

void main()
{
	sVertex vertex = IN_VERTEX.vertex_buffer.vertices[ gl_VertexIndex ];

	gl_Position      = IN_SCENE.view_projection * IN_VERTEX.world_matrix * vec4( vertex.position, 1 );
	OUT.tex_coord_ts = vertex.tex_coord;
}