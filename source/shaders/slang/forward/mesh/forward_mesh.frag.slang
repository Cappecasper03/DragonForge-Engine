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

struct sLight
{
	enum eType
	{
		kAmbient,
		kDirectional,
		kPoint,
	};

	float4 color;
	float3 position;
	float  intensity;
	float3 direction;
	float  radius;
	float3 padding;
	eType  type;
};

struct sFragmentSceneUniforms
{
    Array< sLight, 64 > lights;
    uint                light_count;
};

struct sPushConstant
{
    float4x4 world_matrix;
    float3   camera_position;
};

[ [ vk::binding( 0, 1 ) ] ]
SamplerState SamplerState;

[ [ vk::binding( 1, 1 ) ] ]
Texture2D AlbedoTexture;

[ [ vk::binding( 2, 1 ) ] ]
Texture2D NormalTexture;

[ [ vk::binding( 0, 2 ) ] ]
ConstantBuffer< sFragmentSceneUniforms > SceneUniforms;

[ vk_push_constant ]
sPushConstant PushConstants;

[ shader( "fragment" ) ]
sFragmentOutput main( sFragmentInput IN )
{
    sFragmentOutput OUT;
    float3 color = float3( 0, 0, 0 );

    const float4 albedo_texture = AlbedoTexture.Sample( SamplerState, IN.tex_coord );
    const float3 albedo         = albedo_texture.rgb;

    const float4 normal_texture = NormalTexture.Sample( SamplerState, IN.tex_coord );
    const float3 normal         = mul( IN.tbn_matrix, normalize( normal_texture.rgb * 2 - 1 ) );
    
    for( int i = 0; i < SceneUniforms.light_count; ++i)
    {
        const sLight light       = SceneUniforms.lights[ i ];
        const float3 light_color = light.intensity * light.color.rgb;

        if( light.type == sLight::eType::kAmbient )
        {
            color += albedo.rgb * light_color;
        }
        else if( light.type == sLight::eType::kDirectional )
        {
            const float  diffuse       = max( dot( normal, -light.direction ), 0 );
            const float3 final_diffuse = albedo * diffuse * light_color;

            const float3 vertex_to_camera  = normalize( PushConstants.camera_position - IN.world_position );
            const float3 reflect_direction = reflect( light.direction, normal );
            const float  specular          = pow( max( dot( vertex_to_camera, reflect_direction ), 0 ), 32 );
            const float3 final_specular    = specular * .5f * light_color;

            color += final_diffuse + final_specular;
        }
        else if( light.type == sLight::eType::kPoint )
        {
            const float3 vertex_to_light = light.position - IN.world_position;
            const float3 light_direction = normalize( -vertex_to_light );
            const float  distance  = length( vertex_to_light );

            if( distance > light.radius )
                continue;

            float light_attenuation = 1 - ( distance / light.radius );
            light_attenuation       = light_attenuation * light_attenuation;

            const float  diffuse       = max( dot( normal, -light_direction ), 0 );
            const float3 final_diffuse = albedo * diffuse * light_attenuation * light_color;
            
            const float3 vertex_to_camera  = normalize( PushConstants.camera_position - IN.world_position );
            const float3 reflect_direction = reflect( light_direction, normal );
            const float  specular          = pow( max( dot( vertex_to_camera, reflect_direction ), 0 ), 32 );
            const float3 final_specular    = specular * .5f * light_attenuation * light_color;

            color += final_diffuse + final_specular;
        }
    }

    OUT.color = float4( color, 1 );
    return OUT;
}