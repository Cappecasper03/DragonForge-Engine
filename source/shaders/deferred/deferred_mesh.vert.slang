struct sVertexInput
{
    float3 position  : POSITION;
    float3 normal    : NORMAL;
    float3 tangent   : TANGENT;
    float3 bitangent : BITANGENT;
    float2 tex_coord : TEXCOORD0;
};

struct sVertexOutput
{
    float3   position;
    float2   tex_coord;
    float3x3 tbn;
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

    OUT.position  = mul( SceneUniforms.view_projection, mul( PushConstants.world_matrix, float4( IN.position, 1.0f ) ) ).xyz;
    OUT.tex_coord = IN.tex_coord;
    OUT.tbn       = float3x3( IN.tangent, IN.bitangent, IN.normal );

    return OUT;
}