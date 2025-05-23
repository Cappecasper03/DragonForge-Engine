struct sVertexInput
{
	float2 position;
	float2 tex_coord;
	float4 color;
	float2 size;
	float4 corner_radius;
	float4 border_widths;
	float  is_border;
};

struct sVertexOutput
{
    float4 position : SV_POSITION;
    float2 tex_coord;
    float4 color;
    float2 size;
    float4 corner_radius;
    float4 border_widths;
    float  is_border;
};

struct sVertexSceneUniforms
{
    float4x4 view_projection;
};

#ifdef DF_OpenGL
    layout( binding = 1 )
#endif
#ifdef DF_Vulkan
    [ [ vk::binding( 0, 0 ) ] ]
#endif
ConstantBuffer< sVertexSceneUniforms > SceneUniforms;

[ shader( "vertex" ) ]
sVertexOutput main( sVertexInput IN )
{
    sVertexOutput OUT;

    OUT.position      = mul( SceneUniforms.view_projection, float4( IN.position, 0, 1 ) );
    OUT.tex_coord     = IN.tex_coord;
    OUT.color         = IN.color;
    OUT.size          = IN.size;
    OUT.corner_radius = IN.corner_radius;
    OUT.border_widths = IN.border_widths;
    OUT.is_border     = IN.is_border;

    return OUT;
}