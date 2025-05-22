struct sVertexInput
{
    float2 position;
    float4 color;
};

struct sVertexOutput
{
    float4 position : SV_POSITION;
    float4 color;
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

    OUT.position = mul( SceneUniforms.view_projection, float4( IN.position, 0, 1 ) );
    OUT.color    = IN.color;

    return OUT;
}