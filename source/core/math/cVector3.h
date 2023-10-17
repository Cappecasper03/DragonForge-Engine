#pragma once

#include <complex>

#include "cVector2.h"
#include "core/log/Log.h"

namespace df
{
    template< typename T >
    class cVector3
    {
    public:
        cVector3();

        explicit cVector3( const T& _value );
        cVector3( const T& _x, const T& _y, const T& _z );
        cVector3( const cVector3& _other );
        cVector3( const cVector2< T >& _other, const T& _z );

        cVector3( cVector3&& _other ) = default;
        ~cVector3()                   = default;

        cVector3& operator=( const T& _value );
        cVector3& operator=( const cVector3& _other );
        cVector3& operator=( cVector3&& _other ) = default;

        bool operator==( const cVector3& _other ) { return x == _other.x && y == _other.y && z == _other.z; }
        bool operator!=( const cVector3& _other ) { return x != _other.x || y != _other.y || z != _other.z; }

        cVector3  operator+( const T& _value ) { return cVector3( x + _value, y + _value, z + _value ); }
        cVector3  operator+( const cVector3& _other ) { return cVector3( x + _other.x, y + _other.y, z + _other.z ); }
        cVector3& operator+=( const T& _value );
        cVector3& operator+=( const cVector3& _other );

        cVector3  operator-() { return cVector3( -x, -y, -z ); }
        cVector3  operator-( const T& _value ) { return cVector3( x - _value, y - _value, z - _value ); }
        cVector3  operator-( const cVector3& _other ) { return cVector3( x - _other.x, y - _other.y, z - _other.z ); }
        cVector3& operator-=( const T& _value );
        cVector3& operator-=( const cVector3& _other );

        cVector3  operator*( const T& _value ) { return cVector3( x * _value, y * _value, z * _value ); }
        cVector3  operator*( const cVector3& _other ) { return cVector3( x * _other.x, y * _other.y, z * _other.z ); }
        cVector3& operator*=( const T& _value );
        cVector3& operator*=( const cVector3& _other );

        cVector3  operator/( const T& _value ) { return cVector3( x / _value, y / _value, z / _value ); }
        cVector3  operator/( const cVector3& _other ) { return cVector3( x / _other.x, y / _other.y, z / _other.z ); }
        cVector3& operator/=( const T& _value );
        cVector3& operator/=( const cVector3& _other );

        float operator[]( const size_t& _index );

        double length() { return std::sqrt( std::pow( x, 2 ) + std::pow( y, 2 ) + std::pow( z, 2 ) ); }
        double squaredLength() { return std::pow( x, 2 ) + std::pow( y, 2 ) + std::pow( z, 2 ); }

        cVector3& normalize();
        cVector3  normalized();

        double dot( const cVector3& _other ) { return x * _other.x + y * _other.y + z * _other.z; }
        double dot( const cVector3* _other ) { return x * _other->x + y * _other->y + z * _other->z; }

        double angleBetween( const cVector3& _other ) { return acos( dot( _other ) / ( length() * _other.length() ) ); }
        double angleBetween( const cVector3* _other ) { return acos( dot( _other ) / ( length() * _other->length() ) ); }
        double angleBetweenNormalized( const cVector3& _other ) { return acos( dot( _other ) ); }
        double angleBetweenNormalized( const cVector3* _other ) { return acos( dot( _other ) ); }

        cVector3 cross( const cVector3& _other ) { return cVector3( y * _other.z - z * _other.y, z * _other.x - x * _other.z, x * _other.y - y * _other.x ); }
        cVector3 cross( const cVector3* _other ) { return cVector3( y * _other->z - z * _other->y, z * _other->x - x * _other->z, x * _other->y - y * _other->x ); }

        cVector3 project( const cVector3& _other ) { return _other * ( dot( _other ) / _other.dot( this ) ); }
        cVector3 project( const cVector3* _other ) { return _other * ( dot( _other ) / _other->dot( this ) ); }

        T x;
        T y;
        T z;
    };

    template< typename T >
    cVector3< T >::cVector3()
    : x( 0 ),
      y( 0 ),
      z( 0 )
    {}

    template< typename T >
    cVector3< T >::cVector3( const T& _value )
    : x( _value ),
      y( _value ),
      z( _value )
    {}

    template< typename T >
    cVector3< T >::cVector3( const T& _x, const T& _y, const T& _z )
    : x( _x ),
      y( _y ),
      z( _z )
    {}

    template< typename T >
    cVector3< T >::cVector3( const cVector3& _other )
    : x( _other.x ),
      y( _other.y ),
      z( _other.z )
    {}

    template< typename T >
    cVector3< T >::cVector3( const cVector2< T >& _other, const T& _z )
    : x( _other.x ),
      y( _other.y ),
      z( _z )
    {}

    template< typename T >
    cVector3< T >& cVector3< T >::operator=( const T& _value )
    {
        x = _value;
        y = _value;
        z = _value;
        return *this;
    }

    template< typename T >
    cVector3< T >& cVector3< T >::operator=( const cVector3& _other )
    {
        if( this != &_other )
        {
            x = _other.x;
            y = _other.y;
            z = _other.z;
        }
        return *this;
    }

    template< typename T >
    cVector3< T >& cVector3< T >::operator+=( const T& _value )
    {
        x += _value;
        y += _value;
        z += _value;
        return *this;
    }

    template< typename T >
    cVector3< T >& cVector3< T >::operator+=( const cVector3& _other )
    {
        if( this != &_other )
        {
            x += _other.x;
            y += _other.y;
            z += _other.z;
        }
        return *this;
    }

    template< typename T >
    cVector3< T >& cVector3< T >::operator-=( const T& _value )
    {
        x -= _value;
        y -= _value;
        z -= _value;
        return *this;
    }

    template< typename T >
    cVector3< T >& cVector3< T >::operator-=( const cVector3& _other )
    {
        if( this != &_other )
        {
            x -= _other.x;
            y -= _other.y;
            z -= _other.z;
        }
        return *this;
    }

    template< typename T >
    cVector3< T >& cVector3< T >::operator*=( const T& _value )
    {
        x *= _value;
        y *= _value;
        z *= _value;
        return *this;
    }

    template< typename T >
    cVector3< T >& cVector3< T >::operator*=( const cVector3& _other )
    {
        if( this != &_other )
        {
            x *= _other.x;
            y *= _other.y;
            z *= _other.z;
        }
        return *this;
    }

    template< typename T >
    cVector3< T >& cVector3< T >::operator/=( const T& _value )
    {
        x /= _value;
        y /= _value;
        z /= _value;
        return *this;
    }

    template< typename T >
    cVector3< T >& cVector3< T >::operator/=( const cVector3& _other )
    {
        if( this != &_other )
        {
            x /= _other.x;
            y /= _other.y;
            z /= _other.z;
        }
        return *this;
    }

    template< typename T >
    float cVector3< T >::operator[]( const size_t& _index )
    {
        switch( _index )
        {
            case 0: { return x; }
            case 1: { return y; }
            case 2: { return z; }
            default:
            {
                LOG_WARNING( "Index out of bounds" );
                return 0;
            }
        }
    }

    template< typename T >
    cVector3< T >& cVector3< T >::normalize()
    {
        double len = length();
        x /= len;
        y /= len;
        z /= len;
        return *this;
    }

    template< typename T >
    cVector3< T > cVector3< T >::normalized()
    {
        double len = length();
        if( len != 0 )
            return cVector3( x / len, y / len, z / len );

        return *this;
    }

    typedef cVector3< float > cVector3f;
}
