struct sFragmentInput
{
    float2 tex_coord;
};

struct sFragmentOutput
{
    float4 color : SV_Target;
};

[ [ vk::binding( 0, 1 ) ] ]
SamplerState SamplerState;

[ [ vk::binding( 1, 1 ) ] ]
Texture2D AlbedoTexture;

[ shader( "fragment" ) ]
sFragmentOutput main( sFragmentInput IN )
{
    sFragmentOutput OUT;

    const float4 albedo_texture = AlbedoTexture.Sample( SamplerState, IN.tex_coord );
    const float3 albedo         = albedo_texture.rgb;

    OUT.color = float4( albedo, 1 );

    return OUT;
}