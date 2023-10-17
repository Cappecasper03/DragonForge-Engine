#pragma once

#include "cVector4.h"

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

        cMatrix4X4 operator+( const cMatrix4X4& _other ) { return cMatrix4X4( left + _other.left, up + _other.up, at + _other.at, position + _other.position ); }

        cMatrix4X4& operator+=( const cMatrix4X4& _other );

        cMatrix4X4 operator-( const cMatrix4X4& _other ) { return cMatrix4X4( left.x * _other.left.x + left.y * _other.up.x + left.z * _other.at.x + left.w * _other.position.x ); }

        cMatrix4X4& operator-=( const cMatrix4X4& _other );

        cMatrix4X4 operator*( const cMatrix4X4& _other );

        cMatrix4X4& operator*=( const cMatrix4X4& _other );

        cVector4< T > operator[]( const size_t& _index );

        cMatrix4X4& scale( const cVector3< T >& _scales );
        cMatrix4X4& scale( const T& _scale );

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

    typedef cMatrix4X4< float > cMatrix4X4f;
}
