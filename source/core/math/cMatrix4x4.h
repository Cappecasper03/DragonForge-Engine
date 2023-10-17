#pragma once

#include "cVector4.h"
#include "Math.h"

namespace df
{
    template< typename T >
    class cMatrix4X4
    {
    public:
        enum eCombine
        {
            kReplace,
            kPreMultiply,
            kPostMultiply
        };

        cMatrix4X4();
        cMatrix4X4( const cVector4< T >& _left, const cVector4< T >& _up, const cVector4< T >& _at, const cVector4< T >& _position );
        cMatrix4X4( const cMatrix4X4& _other );

        cMatrix4X4( cMatrix4X4&& _other ) = default;
        ~cMatrix4X4()                     = default;

        cMatrix4X4& operator=( const cMatrix4X4& _other );
        cMatrix4X4& operator=( cMatrix4X4&& _other ) = default;

        cMatrix4X4  operator+( const cMatrix4X4& _other ) { return cMatrix4X4( left + _other.left, up + _other.up, at + _other.at, position + _other.position ); }
        cMatrix4X4& operator+=( const cMatrix4X4& _other );

        cMatrix4X4  operator-( const cMatrix4X4& _other ) { return cMatrix4X4( left.x * _other.left.x + left.y * _other.up.x + left.z * _other.at.x + left.w * _other.position.x ); }
        cMatrix4X4& operator-=( const cMatrix4X4& _other );

        cMatrix4X4  operator*( const cMatrix4X4& _other );
        cMatrix4X4& operator*=( const cMatrix4X4& _other );

        cVector4< T > operator[]( const size_t& _index );

        cMatrix4X4& scale( const cVector3< T >& _scales, const eCombine& _combine = eCombine::kReplace );
        cMatrix4X4& translate( const cVector3< T >& _translation, const eCombine& _combine = eCombine::kReplace );

        cMatrix4X4& rotate( const cVector3< T >& _angles, const eCombine& _combine = eCombine::kReplace );
        cMatrix4X4& rotate( const cVector3< T >& _axis, const T& _angle, const eCombine& _combine = eCombine::kReplace );
        cMatrix4X4& rotateX( const T& _angle, const eCombine& _combine = eCombine::kReplace );
        cMatrix4X4& rotateY( const T& _angle, const eCombine& _combine = eCombine::kReplace );
        cMatrix4X4& rotateZ( const T& _angle, const eCombine& _combine = eCombine::kReplace );

        cMatrix4X4& scaleRotate( const cVector3< T >& _scales, const cVector3< T >& _angles, const eCombine& _combine = eCombine::kReplace );
        cMatrix4X4& scaleTranslate( const cVector3< T >& _scales, const cVector3< T >& _translation, const eCombine& _combine = eCombine::kReplace );
        cMatrix4X4& scaleRotateTranslate( const cVector3< T >& _scales, const cVector3< T >& _angles, const cVector3< T >& _translation, const eCombine& _combine = eCombine::kReplace );
        cMatrix4X4& rotateTranslate( const cVector3< T >& _angles, const cVector3< T >& _translation, const eCombine& _combine = eCombine::kReplace );

        cVector4< T > left;
        cVector4< T > up;
        cVector4< T > at;
        cVector4< T > position;
    };

    template< typename T >
    cMatrix4X4< T >::cMatrix4X4()
    : left( 1, 0, 0, 0 ),
      up( 0, 1, 0, 0 ),
      at( 0, 0, 1, 0 ),
      position( 0, 0, 0, 1 )
    {}

    template< typename T >
    cMatrix4X4< T >::cMatrix4X4( const cVector4< T >& _left, const cVector4< T >& _up, const cVector4< T >& _at, const cVector4< T >& _position )
    : left( _left ),
      up( _up ),
      at( _at ),
      position( _position )
    {}

    template< typename T >
    cMatrix4X4< T >::cMatrix4X4( const cMatrix4X4& _other )
    : left( _other.left ),
      up( _other.up ),
      at( _other.at ),
      position( _other.position )
    {}

    template< typename T >
    cMatrix4X4< T >& cMatrix4X4< T >::operator=( const cMatrix4X4& _other )
    {
        left     = _other.left;
        up       = _other.up;
        at       = _other.at;
        position = _other.position;
        return *this;
    }

    template< typename T >
    cMatrix4X4< T >& cMatrix4X4< T >::operator+=( const cMatrix4X4& _other )
    {
        left += _other.left;
        up += _other.up;
        at += _other.at;
        position += _other.position;
        return *this;
    }

    template< typename T >
    cMatrix4X4< T >& cMatrix4X4< T >::operator-=( const cMatrix4X4& _other )
    {
        left -= _other.left;
        up -= _other.up;
        at -= _other.at;
        position -= _other.position;
        return *this;
    }

    template< typename T >
    cMatrix4X4< T > cMatrix4X4< T >::operator*( const cMatrix4X4& _other )
    {
        cMatrix4X4 out_matrix;

        for( int row = 0; row < 4; ++row )
        {
            for( int column = 0; column < 4; ++column )
            {
                for( int i = 0; i < 4; ++i )
                {
                    out_matrix[ row ][ column ] = this[ row ][ i ] * _other[ i ][ column ];
                }
            }
        }

        return out_matrix;
    }

    template< typename T >
    cMatrix4X4< T >& cMatrix4X4< T >::operator*=( const cMatrix4X4& _other )
    {
        for( int row = 0; row < 4; ++row )
        {
            for( int column = 0; column < 4; ++column )
            {
                for( int i = 0; i < 4; ++i )
                {
                    this[ row ][ column ] = this[ row ][ i ] * _other[ i ][ column ];
                }
            }
        }

        return *this;
    }

    template< typename T >
    cVector4< T > cMatrix4X4< T >::operator[]( const size_t& _index )
    {
        if( _index >= 4 )
        {
            LOG_ERROR( "Index out of bounds" );
            _ASSERT( _index >= 4 );
        }

        switch( _index )
        {
            case 0: { return left; }
            case 1: { return up; }
            case 2: { return at; }
            case 3: { return position; }
            default: return 0;
        }
    }

    template< typename T >
    cMatrix4X4< T >& cMatrix4X4< T >::scale( const cVector3< T >& _scales, const eCombine& _combine )
    {
        if( _combine == eCombine::kReplace )
        {
            left     = cVector4< T >( _scales.x, 0, 0, 0 );
            up       = cVector4< T >( 0, _scales.y, 0, 0 );
            at       = cVector4< T >( 0, 0, _scales.z, 0 );
            position = cVector4< T >( 0, 0, 0, 1 );
        }
        else
        {
            const cMatrix4X4 matrix( cVector4< T >( _scales.x, 0, 0, 0 ),
                                     cVector4< T >( 0, _scales.y, 0, 0 ),
                                     cVector4< T >( 0, 0, _scales.z, 0 ),
                                     cVector4< T >( 0, 0, 0, 1 ) );

            if( _combine == eCombine::kPreMultiply )
                *this = matrix * *this;
            else
                *this = *this * matrix;
        }

        return *this;
    }

    template< typename T >
    cMatrix4X4< T >& cMatrix4X4< T >::translate( const cVector3< T >& _translation, const eCombine& _combine )
    {
        if( _combine == eCombine::kReplace )
        {
            left     = cVector4< T >( 1, 0, 0, 0 );
            up       = cVector4< T >( 0, 1, 0, 0 );
            at       = cVector4< T >( 0, 0, 1, 0 );
            position = cVector4< T >( _translation, 1 );
        }
        else
        {
            const cMatrix4X4 matrix( cVector4< T >( 1, 0, 0, 0 ),
                                     cVector4< T >( 0, 1, 0, 0 ),
                                     cVector4< T >( 0, 0, 1, 0 ),
                                     cVector4< T >( _translation, 1 ) );

            if( _combine == eCombine::kPreMultiply )
                *this = matrix * *this;
            else
                *this = *this * matrix;
        }

        return *this;
    }

    template< typename T >
    cMatrix4X4< T >& cMatrix4X4< T >::rotate( const cVector3< T >& _angles, const eCombine& _combine )
    {
        const T rad_x = math::degToRad( _angles.x );
        const T rad_y = math::degToRad( _angles.y );
        const T rad_z = math::degToRad( _angles.z );
        const T sin_x = std::sin( rad_x );
        const T sin_y = std::sin( rad_y );
        const T sin_z = std::sin( rad_z );
        const T cos_x = std::cos( rad_x );
        const T cos_y = std::cos( rad_y );
        const T cos_z = std::cos( rad_z );

        if( _combine == eCombine::kReplace )
        {
            left     = cVector4< T >( cos_y * cos_z, cos_y * -sin_z, sin_y, 0 );
            up       = cVector4< T >( -sin_x * -sin_y * cos_z + cos_x * sin_z, -sin_x * -sin_y * -sin_z + cos_x * cos_z, -sin_x * cos_y, 0 );
            at       = cVector4< T >( cos_x * -sin_y * cos_z + sin_x * sin_z, cos_x * -sin_y * -sin_z + sin_x * cos_z, cos_x * cos_y, 0 );
            position = cVector4< T >( 0, 0, 0, 1 );
        }
        else
        {
            const cMatrix4X4 matrix( cVector4< T >( cos_y * cos_z, cos_y * -sin_z, sin_y, 0 ),
                                     cVector4< T >( -sin_x * -sin_y * cos_z + cos_x * sin_z, -sin_x * -sin_y * -sin_z + cos_x * cos_z, -sin_x * cos_y, 0 ),
                                     cVector4< T >( cos_x * -sin_y * cos_z + sin_x * sin_z, cos_x * -sin_y * -sin_z + sin_x * cos_z, cos_x * cos_y, 0 ),
                                     cVector4< T >( 0, 0, 0, 1 ) );

            if( _combine == eCombine::kPreMultiply )
                *this = matrix * *this;
            else
                *this = *this * matrix;
        }

        return *this;
    }

    template< typename T >
    cMatrix4X4< T >& cMatrix4X4< T >::rotate( const cVector3< T >& _axis, const T& _angle, const eCombine& _combine )
    {
        const T rad       = math::degToRad( -_angle );
        const T sin       = std::sin( rad );
        const T cos       = std::cos( rad );
        const T tangent   = T( 1 ) - cos;
        const T tangent_x = tangent * _axis.x;
        const T tangent_y = tangent * _axis.y;
        const T tangent_z = tangent * _axis.z;
        const T sin_x     = sin * _axis.x;
        const T sin_y     = sin * _axis.y;
        const T sin_z     = sin * _axis.z;

        if( _combine == eCombine::kReplace )
        {
            left     = cVector4< T >( tangent_x * _axis.x + cos, tangent_x * _axis.y - sin_z, tangent_x * _axis.z + sin_y, 0 );
            up       = cVector4< T >( tangent_y * _axis.x + sin_z, tangent_y * _axis.y + cos, tangent_y * _axis.z - sin_x, 0 );
            at       = cVector4< T >( tangent_z * _axis.x - sin_y, tangent_z * _axis.y + sin_x, tangent_z * _axis.z + cos, 0 );
            position = cVector4< T >( 0, 0, 0, 1 );
        }
        else
        {
            const cMatrix4X4 matrix( cVector4< T >( tangent_x * _axis.x + cos, tangent_x * _axis.y - sin_z, tangent_x * _axis.z + sin_y, 0 ),
                                     cVector4< T >( tangent_y * _axis.x + sin_z, tangent_y * _axis.y + cos, tangent_y * _axis.z - sin_x, 0 ),
                                     cVector4< T >( tangent_z * _axis.x - sin_y, tangent_z * _axis.y + sin_x, tangent_z * _axis.z + cos, 0 ),
                                     cVector4< T >( 0, 0, 0, 1 ) );

            if( _combine == eCombine::kPreMultiply )
                *this = matrix * *this;
            else
                *this = *this * matrix;
        }

        return *this;
    }

    template< typename T >
    cMatrix4X4< T >& cMatrix4X4< T >::rotateX( const T& _angle, const eCombine& _combine )
    {
        if( _combine == eCombine::kReplace )
        {
            left     = cVector4< T >( 1, 0, 0, 0 );
            up       = cVector4< T >( 0, std::cos( _angle ), -std::sin( _angle ), 0 );
            at       = cVector4< T >( 0, std::sin( _angle ), std::cos( _angle ), 0 );
            position = cVector4< T >( 0, 0, 0, 1 );
        }
        else
        {
            const cMatrix4X4 matrix( cVector4< T >( 1, 0, 0, 0 ),
                                     cVector4< T >( 0, std::cos( _angle ), -std::sin( _angle ), 0 ),
                                     cVector4< T >( 0, std::sin( _angle ), std::cos( _angle ), 0 ),
                                     cVector4< T >( 0, 0, 0, 1 ) );

            if( _combine == eCombine::kPreMultiply )
                *this = matrix * *this;
            else
                *this = *this * matrix;
        }

        return *this;
    }

    template< typename T >
    cMatrix4X4< T >& cMatrix4X4< T >::rotateY( const T& _angle, const eCombine& _combine )
    {
        if( _combine == eCombine::kReplace )
        {
            left     = cVector4< T >( std::cos( _angle ), 0, std::sin( _angle ), 0 );
            up       = cVector4< T >( 0, 1, 0, 0 );
            at       = cVector4< T >( -std::sin( _angle ), 0, std::cos( _angle ), 0 );
            position = cVector4< T >( 0, 0, 0, 1 );
        }
        else
        {
            const cMatrix4X4 matrix( cVector4< T >( std::cos( _angle ), 0, std::sin( _angle ), 0 ),
                                     cVector4< T >( 0, 1, 0, 0 ),
                                     cVector4< T >( -std::sin( _angle ), 0, std::cos( _angle ), 0 ),
                                     cVector4< T >( 0, 0, 0, 1 ) );

            if( _combine == eCombine::kPreMultiply )
                *this = matrix * *this;
            else
                *this = *this * matrix;
        }

        return *this;
    }

    template< typename T >
    cMatrix4X4< T >& cMatrix4X4< T >::rotateZ( const T& _angle, const eCombine& _combine )
    {
        if( _combine == eCombine::kReplace )
        {
            left     = cVector4< T >( std::cos( _angle ), -std::sin( _angle ), 0, 0 );
            up       = cVector4< T >( std::sin( _angle ), std::cos( _angle ), 0, 0 );
            at       = cVector4< T >( 0, 0, 1, 0 );
            position = cVector4< T >( 0, 0, 0, 1 );
        }
        else
        {
            const cMatrix4X4 matrix( cVector4< T >( std::cos( _angle ), -std::sin( _angle ), 0, 0 ),
                                     cVector4< T >( std::sin( _angle ), std::cos( _angle ), 0, 0 ),
                                     cVector4< T >( 0, 0, 1, 0 ),
                                     cVector4< T >( 0, 0, 0, 1 ) );

            if( _combine == eCombine::kPreMultiply )
                *this = matrix * *this;
            else
                *this = *this * matrix;
        }

        return *this;
    }

    template< typename T >
    cMatrix4X4< T >& cMatrix4X4< T >::scaleRotate( const cVector3< T >& _scales, const cVector3< T >& _angles, const eCombine& _combine )
    {
        return *this;
    }

    template< typename T >
    cMatrix4X4< T >& cMatrix4X4< T >::scaleTranslate( const cVector3< T >& _scales, const cVector3< T >& _translation, const eCombine& _combine )
    {
        return *this;
    }

    template< typename T >
    cMatrix4X4< T >& cMatrix4X4< T >::scaleRotateTranslate( const cVector3< T >& _scales, const cVector3< T >& _angles, const cVector3< T >& _translation, const eCombine& _combine )
    {
        return *this;
    }

    template< typename T >
    cMatrix4X4< T >& cMatrix4X4< T >::rotateTranslate( const cVector3< T >& _angles, const cVector3< T >& _translation, const eCombine& _combine )
    {
        return *this;
    }

    typedef cMatrix4X4< float > cMatrix4X4f;
}
