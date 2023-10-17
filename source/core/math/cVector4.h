#pragma once

#include <complex>

#include "cVector2.h"
#include "cVector3.h"
#include "core/log/Log.h"

namespace df
{
    template< typename T >
    class cVector4
    {
    public:
        cVector4()
        : x( 0 ),
          y( 0 ),
          z( 0 ),
          w( 0 )
        {}

        explicit cVector4( const T& _value )
        : x( _value ),
          y( _value ),
          z( _value ),
          w( _value )
        {}

        cVector4( const T& _x, const T& _y, const T& _z, const T& _w )
        : x( _x ),
          y( _y ),
          z( _z ),
          w( _w )
        {}

        cVector4( const cVector4& _other )
        : x( _other.x ),
          y( _other.y ),
          z( _other.z ),
          w( _other.w )
        { }

        cVector4( const cVector2< T >& _other, const T& _z, const T& _w )
        : x( _other.x ),
          y( _other.y ),
          z( _z ),
          w( _w )
        { }

        cVector4( const cVector3< T >& _other, const T& _w )
        : x( _other.x ),
          y( _other.y ),
          z( _other.z ),
          w( _w )
        { }

        cVector4( cVector4&& _other ) = default;
        ~cVector4()                   = default;

        cVector4& operator=( const T& _value )
        {
            x = _value;
            y = _value;
            z = _value;
            w = _value;
            return *this;
        }

        cVector4& operator=( const cVector4& _other )
        {
            if( this != &_other )
            {
                x = _other.x;
                y = _other.y;
                z = _other.z;
                w = _other.w;
            }
            return *this;
        }

        cVector4& operator=( cVector4&& _other ) = default;

        cVector4 operator+( const T& _value ) { return cVector4( x + _value, y + _value, z + _value, w + _value ); }
        cVector4 operator+( const cVector4& _other ) { return cVector4( x + _other.x, y + _other.y, z + _other.z, w + _other.w ); }

        cVector4& operator+=( const T& _value )
        {
            x += _value;
            y += _value;
            z += _value;
            w += _value;
            return *this;
        }

        cVector4& operator+=( const cVector4& _other )
        {
            if( this != &_other )
            {
                x += _other.x;
                y += _other.y;
                z += _other.z;
                w += _other.w;
            }
            return *this;
        }

        cVector4 operator-( const T& _value ) { return cVector4( x - _value, y - _value, z - _value, w - _value ); }
        cVector4 operator-( const cVector4& _other ) { return cVector4( x - _other.x, y - _other.y, z - _other.z, w - _other.w ); }

        cVector4& operator-=( const T& _value )
        {
            x -= _value;
            y -= _value;
            z -= _value;
            w -= _value;
            return *this;
        }

        cVector4& operator-=( const cVector4& _other )
        {
            if( this != &_other )
            {
                x -= _other.x;
                y -= _other.y;
                z -= _other.z;
                w -= _other.w;
            }
            return *this;
        }

        cVector4 operator*( const T& _value ) { return cVector4( x * _value, y * _value, z * _value, w * _value ); }
        cVector4 operator*( const cVector4& _other ) { return cVector4( x * _other.x, y * _other.y, z * _other.z, w * _other.w ); }

        cVector4& operator*=( const T& _value )
        {
            x *= _value;
            y *= _value;
            z *= _value;
            w *= _value;
            return *this;
        }

        cVector4& operator*=( const cVector4& _other )
        {
            if( this != &_other )
            {
                x *= _other.x;
                y *= _other.y;
                z *= _other.z;
                w *= _other.w;
            }
            return *this;
        }

        cVector4 operator/( const T& _value ) { return cVector4( x / _value, y / _value, z / _value, w / _value ); }
        cVector4 operator/( const cVector4& _other ) { return cVector4( x / _other.x, y / _other.y, z / _other.z, w / _other.w ); }

        cVector4& operator/=( const T& _value )
        {
            x /= _value;
            y /= _value;
            z /= _value;
            w /= _value;
            return *this;
        }

        cVector4& operator/=( const cVector4& _other )
        {
            if( this != &_other )
            {
                x /= _other.x;
                y /= _other.y;
                z /= _other.z;
                w /= _other.w;
            }
            return *this;
        }

        float operator[]( const unsigned _index )
        {
            if( _index >= 4 )
            {
                LOG_ERROR( "Index out of bounds" );
                _ASSERT( _index >= 4 );
            }

            switch( _index )
            {
                case 0: { return x; }
                case 1: { return y; }
                case 2: { return z; }
                case 3: { return w; }
                default: return 0;
            }
        }

        double length() { return std::sqrt( std::pow( x, 2 ) + std::pow( y, 2 ) + std::pow( z, 2 ) + std::pow( w, 2 ) ); }
        double squaredLength() { return std::pow( x, 2 ) + std::pow( y, 2 ) + std::pow( z, 2 ) + std::pow( w, 2 ); }

        cVector4& normalize()
        {
            double magnitude = length();
            x /= magnitude;
            y /= magnitude;
            z /= magnitude;
            w /= magnitude;
            return *this;
        }

        cVector4 normalized()
        {
            double magnitude = length();
            return cVector4( x / magnitude, y / magnitude, z / magnitude, w / magnitude );
        }

        double dot( const cVector4& _other ) { return x * _other.x + y * _other.y + z * _other.z + w * _other.w; }
        double dot( const cVector4* _other ) { return x * _other->x + y * _other->y + z * _other->z + w * _other->w; }

        double angleBetween( const cVector4& _other ) { return acos( dot( _other ) / ( length() * _other.length() ) ); }
        double angleBetween( const cVector4* _other ) { return acos( dot( _other ) / ( length() * _other->length() ) ); }
        double angleBetweenNormalized( const cVector4& _other ) { return acos( dot( _other ) ); }
        double angleBetweenNormalized( const cVector4* _other ) { return acos( dot( _other ) ); }

        cVector4 project( const cVector4& _other ) { return _other * ( dot( _other ) / _other.dot( this ) ); }
        cVector4 project( const cVector4* _other ) { return _other * ( dot( _other ) / _other->dot( this ) ); }

        T x;
        T y;
        T z;
        T w;
    };

    typedef cVector4< float > cVector4f;
}
