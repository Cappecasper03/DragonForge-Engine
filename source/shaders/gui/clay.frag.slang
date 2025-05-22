struct sFragmentInput
{
    float4 color;
    float2 uv;
    float2 size;
    float4 corner_radii;
};

struct sFragmentOutput
{
    float4 color;
};

float sdRoundedBox( float2 p, float2 b, float4 r )
{
    r.x = min( r.x, min( b.x, b.y ) );
    r.y = min( r.y, min( b.x, b.y ) ); 
    r.z = min( r.z, min( b.x, b.y ) );
    r.w = min( r.w, min( b.x, b.y ) );

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
    const float2 p         = IN.uv * IN.size - half_size;

    float2 local_pos;
    local_pos.x = IN.uv.x * IN.size.x - half_size.x;
    local_pos.y = half_size.y - IN.uv.y * IN.size.y;


    float dist = sdRoundedBox( local_pos, half_size, IN.corner_radii );

    float aa_edge = fwidth( dist );
    float alpha   = smoothstep( aa_edge, -aa_edge, dist );

    OUT.color    = IN.color;
    OUT.color.a *= alpha;

    if ( OUT.color.a < .01 )
        discard;

    return OUT;
}