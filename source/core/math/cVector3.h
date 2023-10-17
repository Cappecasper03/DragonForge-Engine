#pragma once

#include <complex>

#include "cVector2.h"

namespace df
{
    template< typename T >
    class cVector3 final
    {
    public:
        cVector3();

        explicit cVector3( const T& _value );
        cVector3( const T& _x, const T& _y, const T& _z );
        cVector3( const cVector3& _other );
        cVector3( const cVector2< T >& _other, const T& _z );

        cVector3( cVector3&& ) = default;
        ~cVector3()            = default;

        cVector3& operator=( const T& _value );
        cVector3& operator=( const cVector3& _other );
        cVector3& operator=( cVector3&& ) = default;

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

        T length() { return std::sqrt( std::pow( x, 2 ) + std::pow( y, 2 ) + std::pow( z, 2 ) ); }
        T squaredLength() { return std::pow( x, 2 ) + std::pow( y, 2 ) + std::pow( z, 2 ); }

        cVector3& normalize();
        cVector3  normalized();

        T dot( const cVector3& _other ) { return x * _other.x + y * _other.y + z * _other.z; }
        T dot( const cVector3* _other ) { return x * _other->x + y * _other->y + z * _other->z; }

        T angleBetween( const cVector3& _other ) { return acos( dot( _other ) / ( length() * _other.length() ) ); }
        T angleBetween( const cVector3* _other ) { return acos( dot( _other ) / ( length() * _other->length() ) ); }
        T angleBetweenNormalized( const cVector3& _other ) { return acos( dot( _other ) ); }
        T angleBetweenNormalized( const cVector3* _other ) { return acos( dot( _other ) ); }

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
