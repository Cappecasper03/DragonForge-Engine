#pragma once

#include <cmath>

#include "core/log/Log.h"

namespace vg
{
    template< typename T >
    class cVector2
    {
    public:
        cVector2()
        : x( 0 ),
          y( 0 )
        {}

        explicit cVector2( const T& _value )
        : x( _value ),
          y( _value )
        {}

        cVector2( const T& _x, const T& _y )
        : x( _x ),
          y( _y )
        {}

        cVector2( const cVector2& _other )
        : x( _other.x ),
          y( _other.y )
        { }

        cVector2( cVector2&& _other ) = default;
        ~cVector2()                   = default;

        cVector2& operator=( const T& _value )
        {
            x = _value;
            y = _value;
            return *this;
        }

        cVector2& operator=( const cVector2& _other )
        {
            if( this != &_other )
            {
                x = _other.x;
                y = _other.y;
            }
            return *this;
        }

        cVector2& operator=( cVector2&& _other ) = default;

        cVector2 operator+( const T& _value ) { return cVector2( x + _value, y + _value ); }
        cVector2 operator+( const cVector2& _other ) { return cVector2( x + _other.x, y + _other.y ); }

        cVector2& operator+=( const T& _value )
        {
            x += _value;
            y += _value;
            return *this;
        }

        cVector2& operator+=( const cVector2& _other )
        {
            if( this != &_other )
            {
                x += _other.x;
                y += _other.y;
            }
            return *this;
        }

        cVector2 operator-( const T& _value ) { return cVector2( x - _value, y - _value ); }
        cVector2 operator-( const cVector2& _other ) { return cVector2( x - _other.x, y - _other.y ); }

        cVector2& operator-=( const T& _value )
        {
            x -= _value;
            y -= _value;
            return *this;
        }

        cVector2& operator-=( const cVector2& _other )
        {
            if( this != &_other )
            {
                x -= _other.x;
                y -= _other.y;
            }
            return *this;
        }

        cVector2 operator*( const T& _value ) { return cVector2( x * _value, y * _value ); }
        cVector2 operator*( const cVector2& _other ) { return cVector2( x * _other.x, y * _other.y ); }

        cVector2& operator*=( const T& _value )
        {
            x *= _value;
            y *= _value;
            return *this;
        }

        cVector2& operator*=( const cVector2& _other )
        {
            if( this != &_other )
            {
                x *= _other.x;
                y *= _other.y;
            }
            return *this;
        }

        cVector2 operator/( const T& _value ) { return cVector2( x / _value, y / _value ); }
        cVector2 operator/( const cVector2& _other ) { return cVector2( x / _other.x, y / _other.y ); }

        cVector2& operator/=( const T& _value )
        {
            x /= _value;
            y /= _value;
            return *this;
        }

        cVector2& operator/=( const cVector2& _other )
        {
            if( this != &_other )
            {
                x /= _other.x;
                y /= _other.y;
            }
            return *this;
        }

        float operator[]( const unsigned _index )
        {
            if( _index >= 2 )
            {
                LOG_ERROR( "Index out of bounds" );
                _ASSERT( _index >= 4 );
            }

            switch( _index )
            {
                case 0: { return x; }
                case 1: { return y; }
                default: return 0;
            }
        }

        double length() { return sqrt( pow( x, 2 ) + pow( y, 2 ) ); }
        double squaredLength() { return pow( x, 2 ) + pow( y, 2 ); }

        cVector2& normalize()
        {
            double len = length();
            x /= len;
            y /= len;
            return *this;
        }

        cVector2 normalized()
        {
            double len = length();
            if( len != 0 )
                return cVector2( x / len, y / len );

            return *this;
        }

        double dot( const cVector2& _other ) { return x * _other.x + y * _other.y; }
        double dot( const cVector2* _other ) { return x * _other->x + y * _other->y; }

        double angleBetween( const cVector2& _other ) { return acos( dot( _other ) / ( length() * _other.length() ) ); }
        double angleBetween( const cVector2* _other ) { return acos( dot( _other ) / ( length() * _other->length() ) ); }
        double angleBetweenNormalized( const cVector2& _other ) { return acos( dot( _other ) ); }
        double angleBetweenNormalized( const cVector2* _other ) { return acos( dot( _other ) ); }

        cVector2 project( const cVector2& _other ) { return _other * ( dot( _other ) / _other.dot( this ) ); }
        cVector2 project( const cVector2* _other ) { return _other * ( dot( _other ) / _other->dot( this ) ); }

        T x;
        T y;
    };

    typedef cVector2< float > cVector2f;
}
