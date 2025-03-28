struct sFragmentInput
{
    float2 tex_coord;
};

struct sFragmentOutput
{
    float4 color : SV_Target;
};

Texture2D    PositionTexture       : register( b1 );
Texture2D    NormalTexture         : register( b2 );
Texture2D    AlbedoSpecularTexture : register( b3 );

SamplerState SamplerState : register( b4 );

[ shader( "fragment" ) ]
sFragmentOutput main( sFragmentInput IN )
{
    sFragmentOutput OUT;

    const float3 position        = PositionTexture.Sample( SamplerState, IN.tex_coord ).rgb;
    const float3 normal          = NormalTexture.Sample( SamplerState, IN.tex_coord ).rgb;
    const float4 albedo_specular = AlbedoSpecularTexture.Sample( SamplerState, IN.tex_coord );
    const float3 albedo          = albedo_specular.rgb;
    const float  specular        = albedo_specular.a;

    OUT.color = float4( albedo, 1 );

    return OUT;
}