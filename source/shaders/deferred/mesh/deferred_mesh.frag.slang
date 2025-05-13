struct sFragmentInput
{
    float2   tex_coord;
    float3   world_position;
    float3x3 tbn_matrix;
};

struct sFragmentOutput
{
    float3 albedo          : SV_Target0;
    float3 position        : SV_Target1;
    float3 normal          : SV_Target2;
};

struct sPushConstant
{
    float4x4 world_matrix;
    float3   camera_position;
};

#ifdef DF_Vulkan
    [ [ vk::binding( 0, 1 ) ] ]
#endif
SamplerState SamplerState;

#ifdef DF_Vulkan
    [ [ vk::binding( 1, 1 ) ] ]
#endif
Texture2D AlbedoTexture;

#ifdef DF_Vulkan
    [ [ vk::binding( 2, 1 ) ] ]
#endif
Texture2D NormalTexture;

#ifdef DF_OpenGL
    layout( binding = 0 )
#endif
#ifdef DF_Vulkan
    [ vk_push_constant ]
#endif
sPushConstant PushConstants;

[ shader( "fragment" ) ]
sFragmentOutput main( sFragmentInput IN )
{
    sFragmentOutput OUT;

    const float4 albedo_texture = AlbedoTexture.Sample( SamplerState, IN.tex_coord );
    const float4 normal_texture = NormalTexture.Sample( SamplerState, IN.tex_coord ) * 2 - 1;
    const float3 normal         = mul( IN.tbn_matrix, normalize( normal_texture.rgb * 2 - 1 ) );

    OUT.albedo   = albedo_texture.rgb;
    OUT.position = IN.world_position;
    OUT.normal   = normal;

    return OUT;
}