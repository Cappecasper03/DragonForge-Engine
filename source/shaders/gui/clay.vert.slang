struct sVertexInput
{
    uint vertex_id;
};

struct sVertexOutput
{
    float4 position : SV_POSITION;
	float2 tex_coord;
	float4 color;
	float2 size;
	float4 corner_radius;
	float4 border_widths;
	int    type;
};

struct sVertexSceneUniforms
{
    float4x4 view_projection;
};

struct sPushConstants
{
	float2 position[ 6 ];
	float2 tex_coord[ 6 ];
	float4 color;
	float2 size;
	float4 corner_radius;
	float4 border_widths;
	int    type;
};

#ifdef DF_OpenGL
    layout( binding = 1 )
#endif
#ifdef DF_Vulkan
    [ [ vk::binding( 0, 0 ) ] ]
#endif
ConstantBuffer< sVertexSceneUniforms > SceneUniforms;

#ifdef DF_OpenGL
    layout( binding = 0 )
#endif
#ifdef DF_Vulkan
    [ vk_push_constant ]
#endif
sPushConstants PushConstants;

[ shader( "vertex" ) ]
sVertexOutput main( sVertexInput IN )
{
    sVertexOutput OUT;

    OUT.position      = mul( SceneUniforms.view_projection, float4( PushConstants.position[ IN.vertex_id ], 0, 1 ) );
    OUT.tex_coord     = PushConstants.tex_coord[ IN.vertex_id ];
    OUT.color         = PushConstants.color;
    OUT.size          = PushConstants.size;
    OUT.corner_radius = PushConstants.corner_radius;
    OUT.border_widths = PushConstants.border_widths;
    OUT.type          = PushConstants.type;

    return OUT;
}