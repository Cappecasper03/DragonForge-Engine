struct sFragmentInput
{
    float2 tex_coord;
};

struct sFragmentOutput
{
    float4 color : SV_Target;
};

Texture2D    Texture      : register( b1 );
SamplerState SamplerState : register( b2 );

[ shader( "fragment" ) ]
sFragmentOutput main( sFragmentInput IN )
{
    sFragmentOutput OUT;

    OUT.color = Texture.Sample( SamplerState, IN.tex_coord );

    return OUT;
}