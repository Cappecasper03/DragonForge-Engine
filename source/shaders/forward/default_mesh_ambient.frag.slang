struct sFragmentInput
{
    float2 tex_coord : TEXCOORD0;
};

struct sFragmentOutput
{
    float4 color : COLOR0;
};

Texture2D    Texture;
SamplerState SamplerState;

[ shader( "fragment" ) ]
sFragmentOutput main( sFragmentInput IN ) : SV_Target
{
    sFragmentOutput OUT;

    OUT.color = Texture.Sample( SamplerState, IN.tex_coord );

    return OUT;
}