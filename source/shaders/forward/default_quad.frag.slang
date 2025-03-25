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

[vk_push_constant]
sPushConstant PushConstants;

Texture2D    Texture       : register( b1 );
SamplerState SamplerState  : register( b2 );

[ shader( "fragment" ) ]
sFragmentOutput main( sFragmentInput IN ) : SV_Target
{
    sFragmentOutput OUT;

    OUT.color = mul( PushConstants.color, Texture.Sample( SamplerState, IN.tex_coord ) );

    return OUT;
}