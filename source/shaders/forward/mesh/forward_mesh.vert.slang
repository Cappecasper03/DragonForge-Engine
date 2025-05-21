struct sVertexInput
{
    float3 position;
    float3 normal;
    float3 tangent;
    float3 bitangent;
    float2 tex_coord;
};

struct sVertexOutput
{
    float4   position  : SV_POSITION;
    float2   tex_coord;
    float3   world_position;
    float3x3 tbn_matrix;
};

struct sVertexSceneUniforms
{
    float4x4 view_projection;
};

struct sPushConstant
{
    float4x4 world_matrix;
    float3   camera_position;
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

    const float4 position  = mul( PushConstants.world_matrix, float4( IN.position, 1 ) );
    const float3 normal    = normalize( mul( PushConstants.world_matrix, float4( IN.normal, 0 ) ).xyz );
    const float3 tangent   = normalize( mul( PushConstants.world_matrix, float4( IN.tangent, 0 ) ).xyz );
    const float3 bitangent = normalize( mul( PushConstants.world_matrix, float4( IN.bitangent, 0 ) ).xyz );

    OUT.position       = mul( SceneUniforms.view_projection, position );
    OUT.tex_coord      = IN.tex_coord;
    OUT.world_position = position.xyz;
    OUT.tbn_matrix     = transpose( float3x3( IN.tangent, IN.bitangent, IN.normal ) );

    return OUT;
}