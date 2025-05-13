struct sFragmentInput
{
    float2 tex_coord;
    float3 world_position;
};

struct sFragmentOutput
{
    float3 albedo   : SV_Target0;
    float3 position : SV_Target1;
    float3 normal   : SV_Target2;
};

#ifdef DF_Vulkan
    [ [ vk::binding( 0, 1 )] ]
#endif
SamplerState SamplerState;

#ifdef DF_Vulkan
    [ [ vk::binding( 1, 1 )] ]
#endif
Texture2D AlbedoTexture;

[ shader( "fragment" ) ]
sFragmentOutput main( sFragmentInput IN )
{
    sFragmentOutput OUT;

    const float4 albedo_texture = AlbedoTexture.Sample( SamplerState, IN.tex_coord );

    OUT.albedo   = albedo_texture.rgb;
    OUT.position = IN.world_position;
    OUT.normal   = float3( 0, 0, 1 );

    return OUT;
}