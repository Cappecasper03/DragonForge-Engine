struct sFragmentInput
{
    float2 tex_coord : TEXCOORD0;
};

struct sFragmentOutput
{
    float4 color : COLOR0;
};

struct sPushConstant
{
    float4x4 world_matrix;
    float4   color;
};

Texture2D    Texture;
SamplerState SamplerState;

[ shader( "fragment" ) ]
sFragmentOutput main( sFragmentInput IN, sPushConstant PushConstants ) : SV_Target
{
    sFragmentOutput OUT;

    OUT.color = mul( PushConstants.color, Texture.Sample( SamplerState, IN.tex_coord ) );

    return OUT;
}