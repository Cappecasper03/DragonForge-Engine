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
    float4 position  : SV_POSITION;
    float3 normal;
    float2 tex_coord;
    float3 world_position;
};

struct sVertexSceneUniforms
{
    float4x4 view_projection;
};

struct sPushConstant
{
    float4x4 world_matrix;
    float3   camera_position;
    float    padding;
    float3   light_direction;
};

[ [ vk::binding( 0, 0 ) ] ]
ConstantBuffer< sVertexSceneUniforms > SceneUniforms;

[ vk_push_constant ]
sPushConstant PushConstants;

[ shader( "vertex" ) ]
sVertexOutput main( sVertexInput IN )
{
    sVertexOutput OUT;

    const float4 position = mul( PushConstants.world_matrix, float4( IN.position, 1 ) );

    OUT.position       = mul( SceneUniforms.view_projection, position );
    OUT.normal         = IN.normal;
    OUT.tex_coord      = IN.tex_coord;
    OUT.world_position = position.xyz;

    return OUT;
}