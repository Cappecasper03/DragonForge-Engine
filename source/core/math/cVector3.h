#pragma once

#include <cmath>

#include "cVector2.h"

namespace vg
{
    template< typename T >
    class cVector3
    {
    public:
        cVector3()
        : x( 0 ),
          y( 0 ),
          z( 0 )
        {}

        explicit cVector3( const T& _value )
        : x( _value ),
          y( _value ),
          z( _value )
        {}

        cVector3( const T& _x, const T& _y, const T& _z )
        : x( _x ),
          y( _y ),
          z( _z )
        {}

        cVector3( const cVector3& _other )
        : x( _other.x ),
          y( _other.y ),
          z( _other.z )
        { }

        cVector3( const cVector2< T >& _other, const T& _z )
        : x( _other.x ),
          y( _other.y ),
          z( _z )
        { }

        cVector3( cVector3&& _other ) = default;
        ~cVector3()                   = default;

        cVector3& operator=( const T& _value )
        {
            x = _value;
            y = _value;
            z = _value;
            return *this;
        }

        cVector3& operator=( const cVector3& _other )
        {
            if( this != &_other )
            {
                x = _other.x;
                y = _other.y;
                z = _other.z;
            }
            return *this;
        }

        cVector3& operator=( cVector3&& _other ) = default;

        cVector3 operator+( const T& _value ) { return cVector3( x + _value, y + _value, z + _value ); }
        cVector3 operator+( const cVector3& _other ) { return cVector3( x + _other.x, y + _other.y, z + _other.z ); }

        cVector3& operator+=( const T& _value )
        {
            x += _value;
            y += _value;
            z += _value;
            return *this;
        }

        cVector3& operator+=( const cVector3& _other )
        {
            if( this != &_other )
            {
                x += _other.x;
                y += _other.y;
                z += _other.z;
            }
            return *this;
        }

        cVector3 operator-( const T& _value ) { return cVector3( x - _value, y - _value, z - _value ); }
        cVector3 operator-( const cVector3& _other ) { return cVector3( x - _other.x, y - _other.y, z - _other.z ); }

        cVector3& operator-=( const T& _value )
        {
            x -= _value;
            y -= _value;
            z -= _value;
            return *this;
        }

        cVector3& operator-=( const cVector3& _other )
        {
            if( this != &_other )
            {
                x -= _other.x;
                y -= _other.y;
                z -= _other.z;
            }
            return *this;
        }

        cVector3 operator*( const T& _value ) { return cVector3( x * _value, y * _value, z * _value ); }
        cVector3 operator*( const cVector3& _other ) { return cVector3( x * _other.x, y * _other.y, z * _other.z ); }

        cVector3& operator*=( const T& _value )
        {
            x *= _value;
            y *= _value;
            z *= _value;
            return *this;
        }

        cVector3& operator*=( const cVector3& _other )
        {
            if( this != &_other )
            {
                x *= _other.x;
                y *= _other.y;
                z *= _other.z;
            }
            return *this;
        }

        cVector3 operator/( const T& _value ) { return cVector3( x / _value, y / _value, z / _value ); }
        cVector3 operator/( const cVector3& _other ) { return cVector3( x / _other.x, y / _other.y, z / _other.z ); }

        cVector3& operator/=( const T& _value )
        {
            x /= _value;
            y /= _value;
            z /= _value;
            return *this;
        }

        cVector3& operator/=( const cVector3& _other )
        {
            if( this != &_other )
            {
                x /= _other.x;
                y /= _other.y;
                z /= _other.z;
            }
            return *this;
        }

        double length() { return sqrt( pow( x, 2 ) + pow( y, 2 ) + pow( z, 2 ) ); }
        double squaredLength() { return pow( x, 2 ) + pow( y, 2 ) + pow( z, 2 ); }

        cVector3& normalize()
        {
            double len = length();
            x /= len;
            y /= len;
            z /= len;
            return *this;
        }

        cVector3 normalized()
        {
            double len = length();
            if( len != 0 )
                return cVector3( x / len, y / len, z / len );

            return *this;
        }

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

    typedef cVector3< float > cVector3f;
}
