struct sFragmentInput
{
    float4 color;
    float2 object_coord;
};

struct sFragmentOutput
{
    float4 color : SV_Target;
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

float sdRect( float2 _p, float2 _size )
{
    float2 d = abs( _p ) - _size;
    return min( max( d.x, d.y ), 0 ) + length( max( d, 0 ) );
}

// The below function "sdEllipse" is MIT licensed with following text:
//
// The MIT License
// Copyright 2013 Inigo Quilez
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software
// is furnished to do so, subject to the following conditions: The above copyright
// notice and this permission notice shall be included in all copies or substantial
// portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
// EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

float sdEllipse( float2 _p, in float2 ab )
{
    if( abs( ab.x - ab.y ) < .1 )
        return length( _p ) - ab.x;

    _p = abs( _p );

    if( _p.x > _p.y )
    {
        _p = _p.yx;
        ab = ab.yx;
    }

    float l  = ab.y * ab.y - ab.x * ab.x;

    float m  = ab.x * _p.x / l;
    float n  = ab.y * _p.y / l;
    float m2 = m * m;
    float n2 = n * n;

    float c  = ( m2 + n2 - 1 ) / 3;
    float c3 = c * c * c;

    float q  = c3 + m2 * n2 * 2;
    float d  = c3 + m2 * n2;
    float g  = m + m * n2;

    float co;

    if( d < 0 )
    {
        float _p  = acos( q / c3 ) / 3;
        float s   = cos( _p );
        float t   = sin( _p ) * sqrt( 3 );
        float rx  = sqrt( -c * ( s + t + 2 ) + m2 );
        float ry  = sqrt( -c * ( s - t + 2 ) + m2 );
        co        = ( ry + sign( l ) * rx + abs( g ) / ( rx * ry ) - m ) / 2;
    }
    else
    {
        float h   = 2 * m * n * sqrt( d );
        float s   = sign( q + h ) * pow( abs( q + h ), 1 / 3 );
        float u   = sign( q - h ) * pow( abs( q - h ), 1 / 3 );
        float rx  = -s - u - c * 4 + 2 * m2;
        float ry  = ( s - u ) * sqrt( 3 );
        float rm  = sqrt( rx * rx + ry * ry );
        float _p  = ry / sqrt( rm - rx );
        co        = ( _p + 2 * g / rm - m ) / 2;
    }

    float si = sqrt( 1 - co * co );

    float2 r = float2( ab.x * co, ab.y * si );

    return length( r - _p ) * sign( _p.y - r.y );
}

float sdRoundRect( float2 p, float2 size, float4 rx, float4 ry )
{
    size *= .5;
    float2 corner;

    corner = float2( -size.x + rx.x, -size.y + ry.x ); // Top-Left
    float2 local = p - corner;
    if( dot( rx.x, ry.x ) > 0 && p.x < corner.x && p.y <= corner.y )
        return sdEllipse( local, float2( rx.x, ry.x ) );

    corner = float2( size.x - rx.y, -size.y + ry.y ); // Top-Right
    local  = p - corner;
    if( dot( rx.y, ry.y ) > 0 && p.x >= corner.x && p.y <= corner.y )
        return sdEllipse( local, float2( rx.y, ry.y ) );

    corner = float2( size.x - rx.z, size.y - ry.z ); // Bottom-Right
    local  = p - corner;
    if( dot( rx.z, ry.z ) > 0 && p.x >= corner.x && p.y >= corner.y )
        return sdEllipse( local, float2( rx.z, ry.z ) );

    corner = float2( -size.x + rx.w, size.y - ry.w ); // Bottom-Left
    local  = p - corner;
    if( dot( rx.w, ry.w ) > 0 && p.x < corner.x && p.y > corner.y )
        return sdEllipse( local, float2( rx.w, ry.w ) );

    return sdRect( p, size );
}

float2 transformAffine( float2 val, float2 a, float2 b, float2 c )
{
    return val.x * a + val.y * b + c;
}

void unpack( float4 x, out float4 a, out float4 b )
{
    const float s = 65536;
    a             = floor( x / s );
    b             = floor( x - a * s );
}

#define AA_WIDTH 0.354

float antialias( in float d, in float width, in float median )
{
    return smoothstep( median - width, median + width, d );
}

float4 applyClip( float4 _color, float2 _object_coord )
{
    float4 color = _color;

    for( uint i = 0u; i < PushConstants.clip_size; i++ )
    {
        float4x4 data = PushConstants.clip[ i ];
        float2   origin = data[ 0 ].xy;
        float2   size   = data[ 0 ].zw;
        float4   radii_x;
        float4   radii_y;

        unpack( data[ 1 ], radii_x, radii_y );
        bool inverse = bool( data[ 3 ].z );

        float2 p = _object_coord;
        p        = transformAffine( p, data[ 2 ].xy, data[ 2 ].zw, data[ 3 ].xy );
        p -= origin;

        float d_clip = sdRoundRect( p, size, radii_x, radii_y ) * ( inverse ? -1 : 1 );
        float alpha  = antialias( -d_clip, AA_WIDTH, 0 );
        color    = float4( color.rgb * alpha, color.a * alpha );
    }

    return color;
}

[ shader( "fragment" ) ]
sFragmentOutput main( sFragmentInput IN )
{
    sFragmentOutput OUT;

    OUT.color = applyClip( IN.color, IN.object_coord );

    return OUT;
}
