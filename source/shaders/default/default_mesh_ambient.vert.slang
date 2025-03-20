struct VertexInput
{
    float3 position  : POSITION;
    float3 normal    : NORMAL;
    float3 tangent   : TANGENT;
    float3 bitangent : BITANGENT;
    float2 tex_coord : TEXCOORD0;
};

struct VertexOutput
{
    float4 position     : SV_POSITION;
    float2 tex_coord_ts : TEXCOORD0;
};

struct VertexSceneUniforms
{
    float4x4 view_projection;
};

ConstantBuffer< VertexSceneUniforms > IN_SCENE;

struct PushConstant
{
    float4x4 world_matrix;
};

[ shader( "vertex" ) ]
VertexOutput main( VertexInput IN, PushConstant PUSH_CONSTANT )
{
    VertexOutput OUT;

    OUT.position     = mul( IN_SCENE.view_projection, mul( PUSH_CONSTANT.world_matrix, float4( IN.position, 1.0f ) ) );
    OUT.tex_coord_ts = IN.tex_coord;

    return OUT;
}