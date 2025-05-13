struct sFragmentInput
{
    float2 tex_coord;
};

struct sFragmentOutput
{
    float4 color : SV_Target;
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
Texture2D PositionTexture;

#ifdef DF_Vulkan
[ [ vk::binding( 3, 1 ) ] ]
#endif
Texture2D NormalTexture;

[ shader( "fragment" ) ]
sFragmentOutput main( sFragmentInput IN )
{
    sFragmentOutput OUT;

    const float4 albedo_texture   = AlbedoTexture.Sample( SamplerState, IN.tex_coord );
    const float4 position_texture = PositionTexture.Sample( SamplerState, IN.tex_coord );
    const float4 normal_texture   = NormalTexture.Sample( SamplerState, IN.tex_coord );
    
    OUT.color = float4( albedo_texture.rgb, 1 );

    return OUT;
}