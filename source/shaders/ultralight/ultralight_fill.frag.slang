struct sFragmentInput
{
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

struct sFragmentOutput
{
    float4 color : SV_Target;
};

struct sPushConstant
{
    float4      state;
    float4x4    transform;
    float4      scalar4[ 2 ];
    float4      vector[ 8 ];
    uint        clip_size;
    float4x4    clip[ 8 ];
};

#ifdef DF_Vulkan
    [ [ vk::binding( 0, 1 ) ] ]
#endif
SamplerState SamplerState;

#ifdef DF_Vulkan
    [ [ vk::binding( 1, 1 ) ] ]
#endif
Texture2D Texture1;

#ifdef DF_Vulkan
    [ [ vk::binding( 2, 1 ) ] ]
#endif
Texture2D Texture2;

#ifdef DF_Vulkan
    [ [ vk::binding( 3, 1 ) ] ]
#endif
Texture2D Texture3;

#ifdef DF_OpenGL
    layout( binding = 0 )
#endif
#ifdef DF_Vulkan
    [ vk_push_constant ]
#endif
sPushConstant PushConstants;

float scalar( uint _i )
{
    if( _i < 4 )
        return PushConstants.scalar4[ 0 ][ _i ];
    else
        return PushConstants.scalar4[ 1 ][ _i - 4 ];
}

uint   FillType( float4 _data0 ) { return uint( _data0.x + .5 ); }
float4 TileRectUV() { return PushConstants.vector[ 0 ]; }
float2 TileSize() { return PushConstants.vector[ 1 ].zw; }
float2 PatternTransformA() { return PushConstants.vector[ 2 ].xy; }
float2 PatternTransformB() { return PushConstants.vector[ 2 ].zw; }
float2 PatternTransformC() { return PushConstants.vector[ 3 ].xy; }
uint   Gradient_NumStops( float4 _data0 ) { return uint( _data0.y + .5 ); }
bool   Gradient_IsRadial( float4 _data0 ) { return bool( uint( _data0.z + .5 ) ); }
float  Gradient_R0( float4 _data1 ) { return _data1.x; }
float  Gradient_R1( float4 _data1 ) { return _data1.y; }
float2 Gradient_P0( float4 _data1 ) { return _data1.xy; }
float2 Gradient_P1( float4 _data1 ) { return _data1.zw; }
float  SDFMaxDistance( float4 _data0 ) { return _data0.y; }

struct sGradientStop
{
    float  percent;
    float4 color;
};

sGradientStop GetGradientStop( uint _offset, float4 _data2, float4 _data3, float4 _data4, float4 _data5, float4 _data6 )
{
    sGradientStop result;
    if( _offset < 4 )
    {
        result.percent = _data2[ _offset ];
        if( _offset == 0 )
            result.color = _data3;
        else if( _offset == 1 )
            result.color = _data4;
        else if( _offset == 2 )
            result.color = _data5;
        else if( _offset == 3 )
            result.color = _data6;
    }
    else
    {
        result.percent = scalar( _offset - 4 );
        result.color   = PushConstants.vector[ _offset - 4 ];
    }
    return result;
}

#define AA_WIDTH 0.354

float antialias( in float _d, in float _width, in float _median ) { return smoothstep( _median - _width, _median + _width, _d ); }

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

float sdEllipse( float2 _p, in float2 _ab )
{
    if( abs( _ab.x - _ab.y ) < .1 )
        return length( _p ) - _ab.x;

    _p = abs( _p );
    if( _p.x > _p.y )
    {
        _p  = _p.yx;
        _ab = _ab.yx;
    }

    float l = _ab.y * _ab.y - _ab.x * _ab.x;

    float m  = _ab.x * _p.x / l;
    float n  = _ab.y * _p.y / l;
    float m2 = m * m;
    float n2 = n * n;

    float c  = ( m2 + n2 - 1 ) / 3;
    float c3 = c * c * c;

    float q = c3 + m2 * n2 * 2;
    float d = c3 + m2 * n2;
    float g = m + m * n2;

    float co;

    if( d < 0 )
    {
        float _p  = acos( q / c3 ) / 3;
        float s  = cos( _p );
        float t  = sin( _p ) * sqrt( 3 );
        float rx = sqrt( -c * ( s + t + 2 ) + m2 );
        float ry = sqrt( -c * ( s - t + 2 ) + m2 );
        co       = ( ry + sign( l ) * rx + abs( g ) / ( rx * ry ) - m ) / 2;
    }
    else
    {
        float h  = 2 * m * n * sqrt( d );
        float s  = sign( q + h ) * pow( abs( q + h ), 1 / 3 );
        float u  = sign( q - h ) * pow( abs( q - h ), 1 / 3 );
        float rx = -s - u - c * 4 + 2 * m2;
        float ry = ( s - u ) * sqrt( 3 );
        float rm = sqrt( rx * rx + ry * ry );
        float _p  = ry / sqrt( rm - rx );
        co       = ( _p + 2 * g / rm - m ) / 2;
    }

    float si = sqrt( 1 - co * co );

    float2 r = float2( _ab.x * co, _ab.y * si );

    return length( r - _p ) * sign( _p.y - r.y );
}

float sdRoundRect( float2 _p, float2 _size, float4 _rx, float4 _ry )
{
    _size *= .5;
    float2 corner;

    corner = float2( -_size.x + _rx.x, -_size.y + _ry.x ); // Top-Left
    float2 local = _p - corner;
    if( dot( _rx.x, _ry.x ) > 0 && _p.x < corner.x && _p.y <= corner.y )
        return sdEllipse( local, float2( _rx.x, _ry.x ) );

    corner = float2( _size.x - _rx.y, -_size.y + _ry.y ); // Top-Right
    local  = _p - corner;
    if( dot( _rx.y, _ry.y ) > 0 && _p.x >= corner.x && _p.y <= corner.y )
        return sdEllipse( local, float2( _rx.y, _ry.y ) );

    corner = float2( _size.x - _rx.z, _size.y - _ry.z ); // Bottom-Right
    local  = _p - corner;
    if( dot( _rx.z, _ry.z ) > 0 && _p.x >= corner.x && _p.y >= corner.y )
        return sdEllipse( local, float2( _rx.z, _ry.z ) );

    corner = float2( -_size.x + _rx.w, _size.y - _ry.w ); // Bottom-Left
    local  = _p - corner;
    if( dot( _rx.w, _ry.w ) > 0 && _p.x < corner.x && _p.y > corner.y )
        return sdEllipse( local, float2( _rx.w, _ry.w ) );

    return sdRect( _p, _size );
}

float4 fillImage( float2 _uv, float4 _color ) { return Texture1.Sample( SamplerState, _uv ) * _color; }

float2 transformAffine( float2 _val, float2 _a, float2 _b, float2 _c ) { return _val.x * _a + _val.y * _b + _c; }

float4 fillPatternImage( float2 _object_coord, float4 _color )
{
    float4 tile_rect_uv = TileRectUV();
    float2 tile_size    = TileSize();

    float2 p = _object_coord;

    // Apply the affine matrix
    float2 transformed_coords = transformAffine( p, PatternTransformA(), PatternTransformB(), PatternTransformC() );

    // Convert back to uv coordinate space
    transformed_coords /= tile_size;

    // Wrap UVs to [0.0, 1.0] so texture repeats properly
    float2 uv = fract( transformed_coords );

    // Clip to tile-rect UV
    uv *= tile_rect_uv.zw - tile_rect_uv.xy;
    uv += tile_rect_uv.xy;

    return fillImage( uv, _color );
}

// Gradient noise from Jorge Jimenez's presentation:
// http://www.iryoku.com/next-generation-post-processing-in-call-of-duty-advanced-warfare
float gradientNoise( in float2 _uv )
{
    const float3 magic = float3( .06711056, .00583715, 52.9829189 );
    return fract( magic.z * fract( dot( _uv, magic.xy ) ) );
}

float ramp( float _inMin, float _inMax, float _val ) { return clamp( ( _val - _inMin ) / ( _inMax - _inMin ), 0, 1 ); }

float4 fillPatternGradient( float2 _tex_coord, float4 _data0, float4 _data1, float4 _data2, float4 _data3, float4 _data4, float4 _data5, float4 _data6 )
{
    int   num_stops = int( Gradient_NumStops( _data0 ) );
    bool  is_radial = Gradient_IsRadial( _data0 );
    float2 p0       = Gradient_P0( _data1 );
    float2 p1       = Gradient_P1( _data1 );

    float t = 0;
    if( is_radial )
    {
        float  r0     = p1.x;
        float  r1     = p1.y;
        t             = distance( _tex_coord, p0 );
        float  rDelta = r1 - r0;
        t             = clamp( ( t / rDelta ) - ( r0 / rDelta ), 0, 1 );
    }
    else
    {
        float2 V = p1 - p0;
        t        = clamp( dot( _tex_coord - p0, V ) / dot( V, V ), 0, 1 );
    }

    sGradientStop stop0 = GetGradientStop( 0, _data2, _data3, _data4, _data5, _data6 );
    sGradientStop stop1 = GetGradientStop( 1, _data2, _data3, _data4, _data5, _data6 );

    float4 color = lerp( stop0.color, stop1.color, ramp( stop0.percent, stop1.percent, t ) );
    if( num_stops > 2 )
    {
        sGradientStop stop2 = GetGradientStop( 2, _data2, _data3, _data4, _data5, _data6 );
        color              = lerp( color, stop2.color, ramp( stop1.percent, stop2.percent, t ) );
        if( num_stops > 3 )
        {
            sGradientStop stop3 = GetGradientStop( 3, _data2, _data3, _data4, _data5, _data6 );
            color              = lerp( color, stop3.color, ramp( stop2.percent, stop3.percent, t ) );
            if( num_stops > 4 )
            {
                sGradientStop stop4 = GetGradientStop( 4, _data2, _data3, _data4, _data5, _data6 );
                color              = lerp( color, stop4.color, ramp( stop3.percent, stop4.percent, t ) );
                if( num_stops > 5 )
                {
                    sGradientStop stop5 = GetGradientStop( 5, _data2, _data3, _data4, _data5, _data6 );
                    color              = lerp( color, stop5.color, ramp( stop4.percent, stop5.percent, t ) );
                    if( num_stops > 6 )
                    {
                        sGradientStop stop6 = GetGradientStop( 6, _data2, _data3, _data4, _data5, _data6 );
                        color              = lerp( color, stop6.color, ramp( stop5.percent, stop6.percent, t ) );
                    }
                }
            }
        }
    }

    // Add gradient noise to reduce banding (+4/-4 gradations)
    // color += ( 8 / 255 ) * gradientNoise( gl_FragCoord.xy ) - ( 4 / 255 );

    return color;
}

void Unpack( float4 _x, out float4 _a, out float4 _b )
{
    const float s  = 65536;
    _a             = floor( _x / s );
    _b             = floor( _x - _a * s );
}

float antialias2( float _d )
{
    return smoothstep( -AA_WIDTH, +AA_WIDTH, _d );
}

// Returns two values:
// [0] = distance of p to line segment.
// [1] = closest t on line segment, clamped to [0, 1]
float2 sdSegment( float2 _p, float2 _a, float2 b )
{
    float2 pa = _p - _a;
    float2 ba = b - _a;
    float  t  = dot( pa, ba ) / dot( ba, ba );
    return float2( length( pa - ba * t ), t );
}

float testCross( float2 _a, float2 _b, float2 _p ) { return ( _b.y - _a.y ) * ( _p.x - _a.x ) - ( _b.x - _a.x ) * ( _p.y - _a.y ); }

float sdLine( float2 _a, float2 _b, float2 _p )
{
    float2 pa = _p - _a;
    float2 ba = _b - _a;
    float  t  = dot( pa, ba ) / dot( ba, ba );
    return length( pa - ba * t ) * sign( testCross( _a, _b, _p ) );
}

float4 blend( float4 _src, float4 _dest )
{
    float4 result;
    result.rgb = _src.rgb + _dest.rgb * ( 1 - _src.a );
    result.a   = _src.a + _dest.a * ( 1 - _src.a );
    return result;
}

float innerStroke( float _stroke_width, float _d ) { return min( antialias( -_d, AA_WIDTH, 0 ), 1 - antialias( -_d, AA_WIDTH, _stroke_width ) ); }

float4 fillRoundedRect( float2 _tex_coord, float4 _color, float4 _data0, float4 _data1, float4 _data2, float4 _data3, float4 _data4 )
{
    float2 p    = _tex_coord;
    float2 size = _data0.zw;
    p           = ( p - .5 ) * size;
    float d     = sdRoundRect( p, size, _data1, _data2 );

    // Fill background
    float  alpha = antialias( -d, AA_WIDTH, 0 );
    float4 color = _color * alpha;

    // Draw stroke
    float  stroke_width = _data3.x;
    float4 stroke_color = _data4;

    if( stroke_width > 0 )
    {
        alpha         = innerStroke( stroke_width, d );
        float4 stroke = stroke_color * alpha;
        color         = blend( stroke, color );
    }

    return color;
}

float4 fillBoxShadow( float2 _object_coord, float4 _color, float4 _data0, float4 _data1, float4 _data2, float4 _data3, float4 _data4, float4 _data5, float4 _data6 )
{
    float2 p         = _object_coord;
    bool   inset     = bool( uint( _data0.y + .5 ) );
    float  radius    = _data0.z;
    float2 origin    = _data1.xy;
    float2 size      = _data1.zw;
    float2 clip_origin = _data4.xy;
    float2 clip_size = _data4.zw;

    float sdClip  = sdRoundRect( p - clip_origin, clip_size, _data5, _data6 );
    float sdRect  = sdRoundRect( p - origin, size, _data2, _data3 );

    float clip = inset ? -sdRect : sdClip;
    float d    = inset ? -sdClip : sdRect;

    if( clip < 0 )
    {
        discard;
        return float4( 0, 0, 0, 0 );
    }

    float alpha = radius >= 1 ? pow( antialias( -d, radius * 2 + .2, 0 ), 1.9 ) * 3.3 / pow( radius * 1.2, .15 ) : antialias( -d, AA_WIDTH, inset ? -1 : 1 );
    alpha       = clamp( alpha, 0, 1 ) * _color.a;
    return float4( _color.rgb * alpha, alpha );
}

float3 blendOverlay( float3 _src, float3 _dest )
{
    float3 col;
    for( int i = 0; i < 3; ++i )
        col[ i ] = _dest[ i ] < .5 ? ( 2 * _dest[ i ] * _src[ i ] ) : ( 1 - 2 * ( 1 - _dest[ i ] ) * ( 1 - _src[ i ] ) );
    return col;
}

float3 blendColorDodge( float3 _src, float3 _dest )
{
    float3 col;
    for( int i = 0; i < 3; ++i )
        col[ i ] = ( _src[ i ] == 1 ) ? _src[ i ] : min( _dest[ i ] / ( 1 - _src[ i ] ), 1 );
    return col;
}

float3 blendColorBurn( float3 _src, float3 _dest )
{
    float3 col;
    for( int i = 0; i < 3; ++i )
        col[ i ] = ( _src[ i ] == 0 ) ? _src[ i ] : max( ( 1 - ( ( 1 - _dest[ i ] ) / _src[ i ] ) ), 0 );
    return col;
}

float3 blendHardLight( float3 _src, float3 _dest )
{
    float3 col;
    for( int i = 0; i < 3; ++i )
        col[ i ] = _dest[ i ] < .5 ? ( 2 * _dest[ i ] * _src[ i ] ) : ( 1 - 2 * ( 1 - _dest[ i ] ) * ( 1 - _src[ i ] ) );
    return col;
}

float3 blendSoftLight( float3 _src, float3 _dest )
{
    float3 col;
    for( int i = 0; i < 3; ++i )
        col[ i ] = ( _src[ i ] < .5 ) ? ( 2 * _dest[ i ] * _src[ i ] + _dest[ i ] * _dest[ i ] * ( 1 - 2 * _src[ i ] ) )
                                      : ( sqrt( _dest[ i ] ) * ( 2 * _src[ i ] - 1 ) + 2 * _dest[ i ] * ( 1 - _src[ i ] ) );
    return col;
}

float3 rgb2hsl( float3 _col )
{
    const float eps = 0.0000001;
    float       minc = min( _col.r, min( _col.g, _col.b ) );
    float       maxc = max( _col.r, max( _col.g, _col.b ) );
    float3      mask = step( _col.grr, _col.rgb ) * step( _col.bbg, _col.rgb );
    float3      h    = mask * ( float3( 0, 2, 4 ) + ( _col.gbr - _col.brg ) / ( maxc - minc + eps ) ) / 6;
    return float3( fract( 1 + h.x + h.y + h.z ),                          // H
                  ( maxc - minc ) / ( 1 - abs( minc + maxc - 1 ) + eps ), // S
                  ( minc + maxc ) * .5 );                                 // L
}

float3 hsl2rgb( float3 _c )
{
    float3 rgb = clamp( abs( fmod( _c.x * 6 + float3( 0, 4, 2 ), 6 ) - 3 ) - 1, 0, 1 );
    return _c.z + _c.y * ( rgb - 0.5 ) * ( 1 - abs( 2 * _c.z - 1 ) );
}

float3 blendHue( float3 _src, float3 _dest )
{
    float3 baseHSL = rgb2hsl( _dest );
    return hsl2rgb( float3( rgb2hsl( _src ).r, baseHSL.g, baseHSL.b ) );
}

float3 blendSaturation( float3 _src, float3 _dest )
{
    float3 baseHSL = rgb2hsl( _dest );
    return hsl2rgb( float3( baseHSL.r, rgb2hsl( _src ).g, baseHSL.b ) );
}

float3 blendColor( float3 _src, float3 _dest )
{
    float3 blendHSL = rgb2hsl( _src );
    return hsl2rgb( float3( blendHSL.r, blendHSL.g, rgb2hsl( _dest ).b ) );
}

float3 blendLuminosity( float3 _src, float3 _dest )
{
    float3 baseHSL = rgb2hsl( _dest );
    return hsl2rgb( float3( baseHSL.r, baseHSL.g, rgb2hsl( _src ).b ) );
}

float4 saturate( float4 _val ) { return clamp( _val, 0, 1 ); }

float4 fillBlend( float2 _tex_coord, float2 _object_coord, float4 _data0, float4 _color )
{
    const uint BlendOp_Clear      = 0u;
    const uint BlendOp_Source     = 1u;
    const uint BlendOp_Over       = 2u;
    const uint BlendOp_In         = 3u;
    const uint BlendOp_Out        = 4u;
    const uint BlendOp_Atop       = 5u;
    const uint BlendOp_DestOver   = 6u;
    const uint BlendOp_DestIn     = 7u;
    const uint BlendOp_DestOut    = 8u;
    const uint BlendOp_DestAtop   = 9u;
    const uint BlendOp_XOR        = 10u;
    const uint BlendOp_Darken     = 11u;
    const uint BlendOp_Add        = 12u;
    const uint BlendOp_Difference = 13u;
    const uint BlendOp_Multiply   = 14u;
    const uint BlendOp_Screen     = 15u;
    const uint BlendOp_Overlay    = 16u;
    const uint BlendOp_Lighten    = 17u;
    const uint BlendOp_ColorDodge = 18u;
    const uint BlendOp_ColorBurn  = 19u;
    const uint BlendOp_HardLight  = 20u;
    const uint BlendOp_SoftLight  = 21u;
    const uint BlendOp_Exclusion  = 22u;
    const uint BlendOp_Hue        = 23u;
    const uint BlendOp_Saturation = 24u;
    const uint BlendOp_Color      = 25u;
    const uint BlendOp_Luminosity = 26u;

    float4 src   = fillImage( _tex_coord, _color );
    float4 dest  = Texture2.Sample( SamplerState, _object_coord );

    const uint blend_op = uint( _data0.y + .5 );

    if( blend_op == BlendOp_Clear )      return float4( 0, 0, 0, 0 );
    if( blend_op == BlendOp_Source )     return src;
    if( blend_op == BlendOp_Over )       return src + dest * ( 1 - src.a );
    if( blend_op == BlendOp_In )         return src * dest.a;
    if( blend_op == BlendOp_Out )        return src * ( 1 - dest.a );
    if( blend_op == BlendOp_Atop )       return src * dest.a + dest * ( 1 - src.a );
    if( blend_op == BlendOp_DestOver )   return src * ( 1 - dest.a ) + dest;
    if( blend_op == BlendOp_DestIn )     return dest * src.a;
    if( blend_op == BlendOp_DestOut )    return dest * ( 1 - src.a );
    if( blend_op == BlendOp_DestAtop )   return src * ( 1 - dest.a ) + dest * src.a;
    if( blend_op == BlendOp_XOR )        return saturate( src * ( 1 - dest.a ) + dest * ( 1 - src.a ) );
    if( blend_op == BlendOp_Darken )     return float4( min( src.rgb, dest.rgb ) * src.a, dest.a * src.a );
    if( blend_op == BlendOp_Add )        return saturate( src + dest );
    if( blend_op == BlendOp_Difference ) return float4( abs( dest.rgb - src.rgb ) * src.a, dest.a * src.a );
    if( blend_op == BlendOp_Multiply )   return float4( src.rgb * dest.rgb * src.a, dest.a * src.a );
    if( blend_op == BlendOp_Screen )     return float4( ( 1 - ( ( 1 - dest.rgb ) * ( 1 - src.rgb ) ) ) * src.a, dest.a * src.a );
    if( blend_op == BlendOp_Overlay )    return float4( blendOverlay( src.rgb, dest.rgb ) * src.a, dest.a * src.a );
    if( blend_op == BlendOp_Lighten )    return float4( max( src.rgb, dest.rgb ) * src.a, dest.a * src.a );
    if( blend_op == BlendOp_ColorDodge ) return float4( blendColorDodge( src.rgb, dest.rgb ) * src.a, dest.a * src.a );
    if( blend_op == BlendOp_ColorBurn )  return float4( blendColorBurn( src.rgb, dest.rgb ) * src.a, dest.a * src.a );
    if( blend_op == BlendOp_HardLight )  return float4( blendOverlay( dest.rgb, src.rgb ) * src.a, dest.a * src.a );
    if( blend_op == BlendOp_SoftLight )  return float4( blendSoftLight( src.rgb, dest.rgb ) * src.a, dest.a * src.a );
    if( blend_op == BlendOp_Exclusion )  return float4( ( dest.rgb + src.rgb - 2 * dest.rgb * src.rgb ) * src.a, dest.a * src.a );
    if( blend_op == BlendOp_Hue )        return float4( blendHue( src.rgb, dest.rgb ) * src.a, dest.a * src.a );
    if( blend_op == BlendOp_Saturation ) return float4( blendSaturation( src.rgb, dest.rgb ) * src.a, dest.a * src.a );
    if( blend_op == BlendOp_Color )      return float4( blendColor( src.rgb, dest.rgb ) * src.a, dest.a * src.a );
    if( blend_op == BlendOp_Luminosity ) return float4( blendLuminosity( src.rgb, dest.rgb ) * src.a, dest.a * src.a );

    return src;
}

float4 fillMask( float2 _tex_coord, float2 _object_coord, float4 _color )
{
    float4 color = fillImage( _tex_coord, _color );
    float  alpha = Texture2.Sample( SamplerState, _object_coord ).a;
    return color * alpha;
}

float4 fillGlyph( float2 uv, float4 _color, float4 _data0 )
{
    float  alpha         = Texture1.Sample( SamplerState, uv ).r * _color.a;
    alpha               = clamp( alpha, 0, 1 );
    float  fill_color_luma = _data0.y;
    float  corrected_alpha = Texture2.Sample( SamplerState, float2( alpha, fill_color_luma ) ).r;
    // float corrected_alpha = alpha;
    return float4( _color.rgb * corrected_alpha, corrected_alpha );
}

float4 applyClip( float2 _object_coord, float4 _color )
{
    float4 color = _color;

    for( uint i = 0u; i < PushConstants.clip_size; i++ )
    {
        float4x4 data    = PushConstants.clip[ i ];
        float2   origin  = data[ 0 ].xy;
        float2   size    = data[ 0 ].zw;
        float4   radii_x;
        float4   radii_y;
        
        Unpack( data[ 1 ], radii_x, radii_y );
        bool inverse = bool( data[ 3 ].z );

        float2 p = _object_coord;
        p        = transformAffine( p, data[ 2 ].xy, data[ 2 ].zw, data[ 3 ].xy );
        p       -= origin;

        float  d_clip = sdRoundRect( p, size, radii_x, radii_y ) * ( inverse ? -1 : 1 );
        float  alpha  = antialias2( -d_clip );
        color     = float4( color.rgb * alpha, color.a * alpha );

        // if ( abs( d_clip ) < 2 )
        //  color = float4( .9, 1, 0, 1 );
    }

    return color;
}

[ shader( "fragment" ) ]
sFragmentOutput main( sFragmentInput IN )
{
    sFragmentOutput OUT;

    const uint FillType_Solid            = 0u;
    const uint FillType_Image            = 1u;
    const uint FillType_Pattern_Image    = 2u;
    const uint FillType_Pattern_Gradient = 3u;
    const uint FillType_RESERVED_1       = 4u;
    const uint FillType_RESERVED_2       = 5u;
    const uint FillType_RESERVED_3       = 6u;
    const uint FillType_Rounded_Rect     = 7u;
    const uint FillType_Box_Shadow       = 8u;
    const uint FillType_Blend            = 9u;
    const uint FillType_Mask             = 10u;
    const uint FillType_Glyph            = 11u;

    const uint fill_type = FillType( IN.data0 );

    if( fill_type == FillType_Solid )            OUT.color = IN.color;
    if( fill_type == FillType_Image )            OUT.color = fillImage( IN.tex_coord, IN.color );
    if( fill_type == FillType_Pattern_Image )    OUT.color = fillPatternImage( IN.object_coord, IN.color  );
    if( fill_type == FillType_Pattern_Gradient ) OUT.color = fillPatternGradient( IN.tex_coord, IN.data0, IN.data1, IN.data2, IN.data3, IN.data4, IN.data5, IN.data6 );
    if( fill_type == FillType_Rounded_Rect )     OUT.color = fillRoundedRect( IN.tex_coord, IN.color, IN.data0, IN.data1, IN.data2, IN.data3, IN.data4 );
    if( fill_type == FillType_Box_Shadow )       OUT.color = fillBoxShadow( IN.object_coord, IN.color, IN.data0, IN.data1, IN.data2, IN.data3, IN.data4, IN.data5, IN.data6 );
    if( fill_type == FillType_Blend )            OUT.color = fillBlend(  IN.tex_coord, IN.object_coord, IN.data0, IN.color );
    if( fill_type == FillType_Mask )             OUT.color = fillMask( IN.tex_coord, IN.object_coord, IN.color );
    if( fill_type == FillType_Glyph )            OUT.color = fillGlyph( IN.tex_coord, IN.color, IN.data0 );

    OUT.color = applyClip( IN.object_coord, OUT.color );

    return OUT;
}
