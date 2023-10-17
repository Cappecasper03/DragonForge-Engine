﻿#pragma once

#include <complex>

namespace df
{
    template< typename T >
    class cVector2 final
    {
    public:
        cVector2();

        explicit cVector2( const T& _value );
        cVector2( const T& _x, const T& _y );
        cVector2( const cVector2& _other );

        cVector2( cVector2&& ) = default;
        ~cVector2()            = default;

        cVector2& operator=( const T& _value );
        cVector2& operator=( const cVector2& _other );
        cVector2& operator=( cVector2&& ) = default;

        bool operator==( const cVector2& _other ) { return x == _other.x && y == _other.y; }
        bool operator!=( const cVector2& _other ) { return x != _other.x || y != _other.y; }

        cVector2  operator+( const T& _value ) { return cVector2( x + _value, y + _value ); }
        cVector2  operator+( const cVector2& _other ) { return cVector2( x + _other.x, y + _other.y ); }
        cVector2& operator+=( const T& _value );
        cVector2& operator+=( const cVector2& _other );

        cVector2  operator-() { return cVector2( -x, -y ); }
        cVector2  operator-( const T& _value ) { return cVector2( x - _value, y - _value ); }
        cVector2  operator-( const cVector2& _other ) { return cVector2( x - _other.x, y - _other.y ); }
        cVector2& operator-=( const T& _value );
        cVector2& operator-=( const cVector2& _other );

        cVector2  operator*( const T& _value ) { return cVector2( x * _value, y * _value ); }
        cVector2  operator*( const cVector2& _other ) { return cVector2( x * _other.x, y * _other.y ); }
        cVector2& operator*=( const T& _value );
        cVector2& operator*=( const cVector2& _other );

        cVector2  operator/( const T& _value ) { return cVector2( x / _value, y / _value ); }
        cVector2  operator/( const cVector2& _other ) { return cVector2( x / _other.x, y / _other.y ); }
        cVector2& operator/=( const T& _value );
        cVector2& operator/=( const cVector2& _other );

        T length() { return std::sqrt( std::pow( x, 2 ) + std::pow( y, 2 ) ); }
        T squaredLength() { return std::pow( x, 2 ) + std::pow( y, 2 ); }

        cVector2& normalize();
        cVector2  normalized();

        T dot( const cVector2& _other ) { return x * _other.x + y * _other.y; }
        T dot( const cVector2* _other ) { return x * _other->x + y * _other->y; }

        T angleBetween( const cVector2& _other ) { return acos( dot( _other ) / ( length() * _other.length() ) ); }
        T angleBetween( const cVector2* _other ) { return acos( dot( _other ) / ( length() * _other->length() ) ); }
        T angleBetweenNormalized( const cVector2& _other ) { return acos( dot( _other ) ); }
        T angleBetweenNormalized( const cVector2* _other ) { return acos( dot( _other ) ); }

        cVector2 project( const cVector2& _other ) { return _other * ( dot( _other ) / _other.dot( this ) ); }
        cVector2 project( const cVector2* _other ) { return _other * ( dot( _other ) / _other->dot( this ) ); }

        T x;
        T y;
    };

    template< typename T >
    cVector2< T >::cVector2()
    : x( 0 ),
      y( 0 )
    {}

    template< typename T >
    cVector2< T >::cVector2( const T& _value )
    : x( _value ),
      y( _value )
    {}

    template< typename T >
    cVector2< T >::cVector2( const T& _x, const T& _y )
    : x( _x ),
      y( _y )
    {}

    template< typename T >
    cVector2< T >::cVector2( const cVector2& _other )
    : x( _other.x ),
      y( _other.y )
    {}

    template< typename T >
    cVector2< T >& cVector2< T >::operator=( const T& _value )
    {
        x = _value;
        y = _value;
        return *this;
    }

    template< typename T >
    cVector2< T >& cVector2< T >::operator=( const cVector2& _other )
    {
        if( this != &_other )
        {
            x = _other.x;
            y = _other.y;
        }
        return *this;
    }

    template< typename T >
    cVector2< T >& cVector2< T >::operator+=( const T& _value )
    {
        x += _value;
        y += _value;
        return *this;
    }

    template< typename T >
    cVector2< T >& cVector2< T >::operator+=( const cVector2& _other )
    {
        if( this != &_other )
        {
            x += _other.x;
            y += _other.y;
        }
        return *this;
    }

    template< typename T >
    cVector2< T >& cVector2< T >::operator-=( const T& _value )
    {
        x -= _value;
        y -= _value;
        return *this;
    }

    template< typename T >
    cVector2< T >& cVector2< T >::operator-=( const cVector2& _other )
    {
        if( this != &_other )
        {
            x -= _other.x;
            y -= _other.y;
        }
        return *this;
    }

    template< typename T >
    cVector2< T >& cVector2< T >::operator*=( const T& _value )
    {
        x *= _value;
        y *= _value;
        return *this;
    }

    template< typename T >
    cVector2< T >& cVector2< T >::operator*=( const cVector2& _other )
    {
        if( this != &_other )
        {
            x *= _other.x;
            y *= _other.y;
        }
        return *this;
    }

    template< typename T >
    cVector2< T >& cVector2< T >::operator/=( const T& _value )
    {
        x /= _value;
        y /= _value;
        return *this;
    }

    template< typename T >
    cVector2< T >& cVector2< T >::operator/=( const cVector2& _other )
    {
        if( this != &_other )
        {
            x /= _other.x;
            y /= _other.y;
        }
        return *this;
    }

    template< typename T >
    cVector2< T >& cVector2< T >::normalize()
    {
        double len = length();
        x /= len;
        y /= len;
        return *this;
    }

    template< typename T >
    cVector2< T > cVector2< T >::normalized()
    {
        double len = length();
        if( len != 0 )
            return cVector2( x / len, y / len );

        return *this;
    }

    typedef cVector2< float > cVector2f;
}
