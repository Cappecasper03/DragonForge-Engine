#define DF_MaxLights 512

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
		kSpot,
	};

	float4  color;
	float3  position;
	float   intensity;
	float3  direction;
	float   radius;
	float   spot_inner_angle;
	float   spot_outer_angle;
	float2  padding0;
	float3  padding1;
	eType   type;
};

struct sFragmentSceneUniforms
{
    Array< sLight, DF_MaxLights > lights;
    uint                          light_count;
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

#ifdef DF_OpenGL
    layout( binding = 2 )
#else
    [ [ vk::binding( 0, 2 ) ] ]
#endif
ConstantBuffer< sFragmentSceneUniforms > SceneUniforms;

#ifdef DF_OpenGL
    layout( binding = 0 )
#else
    [ vk_push_constant ]
#endif
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
            const float  distance        = length( vertex_to_light );
            
            if( distance > light.radius )
                continue;

            float light_attenuation = 1 - ( distance / light.radius );
            light_attenuation       = light_attenuation * light_attenuation;
            
            const float3 light_direction = normalize( -vertex_to_light );

            const float  diffuse       = max( dot( normal, -light_direction ), 0 );
            const float3 final_diffuse = albedo * diffuse * light_color;
            
            const float3 vertex_to_camera  = normalize( PushConstants.camera_position - IN.world_position );
            const float3 reflect_direction = reflect( light_direction, normal );
            const float  specular          = pow( max( dot( vertex_to_camera, reflect_direction ), 0 ), 32 );
            const float3 final_specular    = specular * .5f * light_color;

            color += ( final_diffuse + final_specular ) * light_attenuation;
        }
        else if(light.type == sLight::eType::kSpot)
        {
            const float3 vertex_to_light = light.position - IN.world_position;
            const float  distance        = length( vertex_to_light );
            
            if( distance > light.radius )
                continue;

            const float3 light_direction = normalize( -vertex_to_light );

            const float theta = dot( light_direction, -light.direction );

            if( theta < light.spot_outer_angle )
                continue;

            float light_attenuation = 1 - ( distance / light.radius );
            light_attenuation       = light_attenuation * light_attenuation;

            const float epsilon   = light.spot_inner_angle - light.spot_outer_angle;
            const float intensity = clamp( ( theta - light.spot_outer_angle ) / epsilon, 0, 1 );

            const float  diffuse       = max( dot( normal, -light_direction ), 0 );
            const float3 final_diffuse = albedo * diffuse * light_color;
            
            const float3 vertex_to_camera  = normalize( PushConstants.camera_position - IN.world_position );
            const float3 reflect_direction = reflect( light_direction, normal );
            const float  specular          = pow( max( dot( vertex_to_camera, reflect_direction ), 0 ), 32 );
            const float3 final_specular    = specular * .5f * light_color;

            color += ( final_diffuse + final_specular ) * light_attenuation * intensity;
        }
    }

    OUT.color = float4( color, 1 );
    return OUT;
}