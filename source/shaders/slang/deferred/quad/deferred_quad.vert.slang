struct sVertexInput
{
    float3 position  : POSITION;
    float2 tex_coord : TEXCOORD0;
};

struct sVertexOutput
{
    float4   sv_position : SV_POSITION;
    float3   position;
    float2   tex_coord;
};

struct sVertexSceneUniforms
{
    float4x4 view_projection;
};

struct sPushConstant
{
    float4x4 world_matrix;
};

ConstantBuffer< sVertexSceneUniforms > SceneUniforms : register( b0 );

[vk_push_constant]
sPushConstant PushConstants;

[ shader( "vertex" ) ]
sVertexOutput main( sVertexInput IN )
{
    sVertexOutput OUT;

    OUT.sv_position = mul( SceneUniforms.view_projection, mul( PushConstants.world_matrix, float4( IN.position, 1.0f ) ) );
    OUT.position    = mul( PushConstants.world_matrix, float4( IN.position, 1.0f ) ).xyz;
    OUT.tex_coord   = IN.tex_coord;

    return OUT;
}