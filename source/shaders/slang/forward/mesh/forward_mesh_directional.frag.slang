struct sFragmentInput
{
    float3 normal;
    float2 tex_coord;
    float3 world_position;
};

struct sFragmentOutput
{
    float4 color : SV_Target;
};

struct sPushConstant
{
    float4x4 world_matrix;
    float3   camera_position;
    float    padding;
    float3   light_direction;
};

[ [ vk::binding( 0, 1 ) ] ]
Texture2D Texture;

[ [ vk::binding( 1, 1 ) ] ]
SamplerState SamplerState;

[ vk_push_constant ]
sPushConstant PushConstants;

[ shader( "fragment" ) ]
sFragmentOutput main( sFragmentInput IN )
{
    sFragmentOutput OUT;

    const float4 ambient = Texture.Sample( SamplerState, IN.tex_coord );
    const float  diffuse = mul( max( dot( IN.normal, PushConstants.light_direction ), 0.0f ), float4( 1, 0, 0, 0 ) );

    const float3 vertex_to_camera  = normalize( PushConstants.camera_position - IN.world_position );
    const float3 reflect_direction = reflect( -PushConstants.light_direction, IN.normal );
    const float  specular          = mul( pow( max( dot( vertex_to_camera, reflect_direction ), 0.0f ), 32.0f ), float4( 1, 0, 0, 0 ) );

    OUT.color = ambient + diffuse + specular;

    return OUT;
}