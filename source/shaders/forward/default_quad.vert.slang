struct sVertexInput
{
    float3 position  : POSITION;
    float2 tex_coord : TEXCOORD0;
};

struct sVertexOutput
{
    float4 position  : SV_POSITION;
    float2 tex_coord : TEXCOORD0;
};

struct sVertexSceneUniforms
{
    float4x4 view_projection;
};

ConstantBuffer< sVertexSceneUniforms > SceneUniforms;

struct sPushConstant
{
    float4x4 world_matrix;
    float4   color;
};

[ shader( "vertex" ) ]
sVertexOutput main( sVertexInput IN, sPushConstant PushConstants )
{
    sVertexOutput OUT;

    OUT.position  = mul( SceneUniforms.view_projection, mul( PushConstants.world_matrix, float4( IN.position, 1.0f ) ) );
    OUT.tex_coord = IN.tex_coord;

    return OUT;
}