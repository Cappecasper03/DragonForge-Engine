struct sFragmentInput
{
    float2 tex_coord;
    float4 color;
    float2 size;
    float4 corner_radii;
    float4 border_widths;
    float  is_border;
};

struct sFragmentOutput
{
    float4 color;
};

float sdRoundedBox( float2 p, float2 b, float4 r )
{
    float min_b = min( b.x, b.y );
    r.x         = min( r.x, min_b );
    r.y         = min( r.y, min_b ); 
    r.z         = min( r.z, min_b );
    r.w         = min( r.w, min_b );
    r           = max( r, 0 );

    float current_r;
    if( p.x > 0 )
    {
        if( p.y > 0 )
            current_r = r.y;
        else
            current_r = r.z;
    } else
    {
        if( p.y > 0 )
            current_r = r.x;
        else
            current_r = r.w;
    }

    float2 q = abs( p ) - b + current_r;
    return min( max( q.x, q.y ), 0 ) + length( max( q, 0 ) ) - current_r;
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
    if( IN.is_border > .5 )
    {
        float border_T = IN.border_widths.x;
        float border_R = IN.border_widths.y;
        float border_B = IN.border_widths.z;
        float border_L = IN.border_widths.w;

        float dist_outer = sdRoundedBox( local_pos, half_size, IN.corner_radii );

        float2 inner_half_size;
        inner_half_size.x = max( 0, half_size.x - ( border_L + border_R ) * .5 );
        inner_half_size.y = max( 0, half_size.y - ( border_T + border_B ) * .5 );

        float4 inner_radii = float4(
            max( 0, IN.corner_radii.x - max( border_L, border_T ) ),
            max( 0, IN.corner_radii.y - max( border_R, border_T ) ),
            max( 0, IN.corner_radii.z - max( border_R, border_B ) ),
            max( 0, IN.corner_radii.w - max( border_L, border_B ) )
        );
        
        float dist_inner_hole = sdRoundedBox( local_pos, inner_half_size, inner_radii );

        dist = max( dist_outer, -dist_inner_hole );
    }
    else
        dist = sdRoundedBox( local_pos, half_size, IN.corner_radii );

    float aa_edge = fwidth( dist );
    float alpha   = smoothstep( aa_edge, -aa_edge, dist );

    OUT.color    = IN.color;
    OUT.color.a *= alpha;

    if ( OUT.color.a < .01 )
        discard;

    return OUT;
}