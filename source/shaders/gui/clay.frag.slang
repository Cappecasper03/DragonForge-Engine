struct sFragmentInput
{
    float4 color;
};

struct sFragmentOutput
{
    float4 color;
};

[ shader( "fragment" ) ]
sFragmentOutput main( sFragmentInput IN )
{
    sFragmentOutput OUT;

    OUT.color = IN.color;

    return OUT;
}