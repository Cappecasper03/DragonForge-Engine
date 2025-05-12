struct sVertexInput
{
    float3 position  : POSITION;
    float2 tex_coord : TEXCOORD0;
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

[ [ vk::binding( 0, 0 ) ] ]
ConstantBuffer< sVertexSceneUniforms > SceneUniforms;

[ vk_push_constant ]
sPushConstant PushConstants;

[ shader( "vertex" ) ]
sVertexOutput main( sVertexInput IN )
{
    sVertexOutput OUT;

    OUT.position  = mul( SceneUniforms.view_projection, mul( PushConstants.world_matrix, float4( IN.position, 1 ) ) );
    OUT.tex_coord = IN.tex_coord;

    return OUT;
}