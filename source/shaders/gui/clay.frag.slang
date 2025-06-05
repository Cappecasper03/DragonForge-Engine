struct sFragmentInput
{
    float2 tex_coord;
    float4 color;
    float2 size;
    float4 corner_radius;
    float4 border_widths;
    int    type;
};

enum eGuiType
{
    kRectangle,
    kBorder,
    kImage,
};

struct sFragmentOutput
{
    float4 color;
};

[ [ vk::binding( 0, 0 ) ] ]
SamplerState SamplerState;

[ [ vk::binding( 1, 0 ) ] ]
Texture2D AlbedoTexture;

float sdRoundedBox( float2 _point, float2 _half_extent, float4 _radius )
{
    _radius = max( _radius, 0 );

    _radius.xy = _point.x > 0 ? _radius.yz : _radius.xw;
    _radius.x = _point.y > 0 ? _radius.x : _radius.y;

    const float2 q = abs( _point ) - _half_extent + _radius.x;
    return min( max( q.x, q.y ), 0 ) + length( max( q, 0 ) ) - _radius.x;
}

[ shader( "fragment" ) ]
sFragmentOutput main( sFragmentInput IN )
{
    sFragmentOutput OUT;

    const float2 half_size = IN.size * .5;

    float2 local_pos;
    local_pos.x = IN.tex_coord.x * IN.size.x - half_size.x;
    local_pos.y = half_size.y - IN.tex_coord.y * IN.size.y;

    float dist;
    if( IN.type == eGuiType::kRectangle )
        dist = sdRoundedBox( local_pos, half_size, IN.corner_radius );
    else if( IN.type == eGuiType::kBorder )
    {
        const float left_width   = IN.border_widths.x;
        const float right_width  = IN.border_widths.y;
        const float top_width    = IN.border_widths.z;
        const float bottom_width = IN.border_widths.w;

        float2 inner_half_size;
        inner_half_size.x = max( 0, half_size.x - ( left_width + right_width ) * .5 );
        inner_half_size.y = max( 0, half_size.y - ( top_width + bottom_width ) * .5 );
        
        float4 inner_radius = float4(
            max( 0, IN.corner_radius.x - max( left_width, top_width ) ),
            max( 0, IN.corner_radius.y - max( right_width, top_width ) ),
            max( 0, IN.corner_radius.z - max( right_width, bottom_width ) ),
            max( 0, IN.corner_radius.w - max( left_width, bottom_width ) )
        );

        const float dist_outer = sdRoundedBox( local_pos, half_size, IN.corner_radius );
        const float dist_inner = sdRoundedBox( local_pos, inner_half_size, inner_radius );

        dist = max( dist_outer, -dist_inner );
    }
    else if( IN.type == eGuiType::kImage )
    {
        dist     = sdRoundedBox( local_pos, half_size, IN.corner_radius );
        IN.color = AlbedoTexture.Sample( SamplerState, IN.tex_coord ) * IN.color;
    }
    else
        discard;

    float aa_edge = fwidth( dist );
    float alpha   = smoothstep( aa_edge, -aa_edge, dist );

    OUT.color    = IN.color;
    OUT.color.a *= alpha;

    if ( OUT.color.a < .01 )
        discard;

    return OUT;
}