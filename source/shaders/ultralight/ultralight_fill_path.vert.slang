struct sVertexInput
{
  float2 position;
  float4 color;
  float2 tex_coord;
};

struct sVertexOutput
{
  float4 position : SV_POSITION;
  float4 color;
  float2 object_coord;
};

struct sPushConstant
{
    float4   state;
    float4x4 transform;
    float4   scalar4[ 2 ];
    float4   vector[ 8 ];
    uint     clip_size;
    float4x4 clip[ 8 ];
};

#ifdef DF_OpenGL
    layout( binding = 0 )
#endif
#ifdef DF_Vulkan
    [ vk_push_constant ]
#endif
sPushConstant PushConstants;

[ shader( "vertex" ) ]
sVertexOutput main( sVertexInput IN )
{
    sVertexOutput OUT;

    OUT.position     = mul( PushConstants.transform, float4( IN.position, 0, 1 ) );
    OUT.color        = IN.color;
    OUT.object_coord = IN.tex_coord;

    return OUT;
}