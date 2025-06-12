struct sVertexInput
{
    float3 position;
    float2 tex_coord;
};

struct sVertexOutput
{
    float4 position : SV_POSITION;
    float2 tex_coord;
};

struct sVertexSceneUniforms
{
    float4x4 view_projection;
};

struct sPushConstant
{
    float4x4 world_matrix;
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
sPushConstant PushConstants;

[ shader( "vertex" ) ]
sVertexOutput main( sVertexInput IN )
{
    sVertexOutput OUT;

    OUT.position  = mul( SceneUniforms.view_projection, mul( PushConstants.world_matrix, float4( IN.position, 1 ) ) );
    OUT.tex_coord = IN.tex_coord;

    return OUT;
}