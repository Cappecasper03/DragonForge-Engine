struct sFragmentInput
{
    float2 tex_coord;
};

struct sFragmentOutput
{
    float4 color : SV_Target;
};

[ [ vk::binding( 0, 1 ) ] ]
Texture2D Texture;

[ [ vk::binding( 1, 1 ) ] ]
SamplerState SamplerState;

[ shader( "fragment" ) ]
sFragmentOutput main( sFragmentInput IN )
{
    sFragmentOutput OUT;

    OUT.color = Texture.Sample( SamplerState, IN.tex_coord );

    return OUT;
}