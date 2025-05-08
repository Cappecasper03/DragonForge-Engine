struct sFragmentInput
{
    float2   tex_coord;
    float3   world_position;
    float3x3 tbn_matrix;
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
SamplerState SamplerState;

[ [ vk::binding( 1, 1 ) ] ]
Texture2D AlbedoTexture;

[ [ vk::binding( 2, 1 ) ] ]
Texture2D NormalTexture;

[ vk_push_constant ]
sPushConstant PushConstants;

[ shader( "fragment" ) ]
sFragmentOutput main( sFragmentInput IN )
{
    sFragmentOutput OUT;

    const float3 ambient = AlbedoTexture.Sample( SamplerState, IN.tex_coord ).rgb;
    
    float3 normal = NormalTexture.Sample( SamplerState, IN.tex_coord ).rgb;
    normal        = normal * 2 - 1;
    normal        = normalize( mul( IN.tbn_matrix, normal ) );

    const float  diffuse = max( dot( normal, -PushConstants.light_direction ), 0 );

    const float3 vertex_to_camera  = normalize( PushConstants.camera_position - IN.world_position );
    const float3 reflect_direction = reflect( PushConstants.light_direction, normal );
    const float  specular          = pow( max( dot( vertex_to_camera, reflect_direction ), 0 ), 32 );

    OUT.color = float4( ( ambient * diffuse ) + ( ambient * specular * .5f ), 1 );

    return OUT;
}