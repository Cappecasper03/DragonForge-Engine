struct sFragmentInput
{
    float3   position;
    float2   tex_coord;
};

struct sFragmentOutput
{
    float3 position        : SV_Target0;
    float3 normal          : SV_Target1;
    float4 albedo_specular : SV_Target2;
};

Texture2D Texture : register( b1 );

SamplerState SamplerState : register( b2 );

[ shader( "fragment" ) ]
sFragmentOutput main( sFragmentInput IN )
{
    sFragmentOutput OUT;

    OUT.position            = IN.position;
    OUT.normal              = float3( 0, 0, 1 );
    OUT.albedo_specular.rgb = Texture.Sample( SamplerState, IN.tex_coord ).rgb;
    OUT.albedo_specular.a   = 0;

    return OUT;
}