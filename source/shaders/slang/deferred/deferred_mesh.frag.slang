struct sFragmentInput
{
    float3   position;
    float2   tex_coord;
    float3x3 tbn;
};

struct sFragmentOutput
{
    float3 position        : SV_Target0;
    float3 normal          : SV_Target1;
    float4 albedo_specular : SV_Target2;
};

Texture2D    AlbedoTexture   : register( b1 );
Texture2D    NormalTexture   : register( b2 );
Texture2D    SpecularTexture : register( b3 );

SamplerState SamplerState : register( b4 );

[ shader( "fragment" ) ]
sFragmentOutput main( sFragmentInput IN )
{
    sFragmentOutput OUT;

    const float3 normal_texture = NormalTexture.Sample( SamplerState, IN.tex_coord ).rgb * 2 - 1;
    const float3 normal         = ( normalize( mul( IN.tbn, normal_texture ) ) + 1 ) / 2;

    OUT.position            = IN.position;
    OUT.normal              = normal;
    OUT.albedo_specular.rgb = AlbedoTexture.Sample( SamplerState, IN.tex_coord ).rgb;
    OUT.albedo_specular.a   = SpecularTexture.Sample( SamplerState, IN.tex_coord ).r;

    return OUT;
}