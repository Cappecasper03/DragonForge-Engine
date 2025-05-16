struct sVertexInput
{
    float2 position;
    float4 color;
    float2 tex_coord;
    float2 object_coord;
    float4 data0;
    float4 data1;
    float4 data2;
    float4 data3;
    float4 data4;
    float4 data5;
    float4 data6;
};

struct sVertexOutput
{
    float4 position : SV_POSITION;
    float4 color;
    float2 tex_coord;
    float4 data0;
    float4 data1;
    float4 data2;
    float4 data3;
    float4 data4;
    float4 data5;
    float4 data6;
    float2 object_coord;
    float2 screen_coord;
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
    OUT.tex_coord    = IN.tex_coord;
    OUT.data0        = IN.data0;
    OUT.data1        = IN.data1;
    OUT.data2        = IN.data2;
    OUT.data3        = IN.data3;
    OUT.data4        = IN.data4;
    OUT.data5        = IN.data5;
    OUT.data6        = IN.data6;
    OUT.object_coord = IN.object_coord;
    OUT.screen_coord = IN.tex_coord;

    return OUT;
}