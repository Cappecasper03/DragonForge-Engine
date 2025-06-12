struct sVertexInput
{
    uint vertex_id : SV_VertexID;
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
	float4 position_tex_coord[ 6 ];
	float4 color;
	float4 corner_radius;
	float4 border_widths;
	float2 size;
    float  padding1;
    float  padding2;
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

    OUT.position      = mul( SceneUniforms.view_projection, float4( PushConstants.position_tex_coord[ IN.vertex_id ].xy, 0, 1 ) );
    OUT.tex_coord     = PushConstants.position_tex_coord[ IN.vertex_id ].zw;
    OUT.color         = PushConstants.color;
    OUT.size          = PushConstants.size;
    OUT.corner_radius = PushConstants.corner_radius;
    OUT.border_widths = PushConstants.border_widths;
    OUT.type          = PushConstants.type;

    return OUT;
}