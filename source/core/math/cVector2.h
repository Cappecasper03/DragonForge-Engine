#pragma once

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

        bool operator==( const cVector2& _other ) { return x() == _other.x() && y() == _other.y(); }
        bool operator!=( const cVector2& _other ) { return x() != _other.x() || y() != _other.y(); }

        cVector2  operator+( const T& _value ) { return cVector2( x() + _value, y() + _value ); }
        cVector2  operator+( const cVector2& _other ) { return cVector2( x() + _other.x(), y() + _other.y() ); }
        cVector2& operator+=( const T& _value );
        cVector2& operator+=( const cVector2& _other );

        cVector2  operator-() { return cVector2( -x(), -y() ); }
        cVector2  operator-( const T& _value ) { return cVector2( x() - _value, y() - _value ); }
        cVector2  operator-( const cVector2& _other ) { return cVector2( x() - _other.x(), y() - _other.y() ); }
        cVector2& operator-=( const T& _value );
        cVector2& operator-=( const cVector2& _other );

        cVector2  operator*( const T& _value ) { return cVector2( x() * _value, y() * _value ); }
        cVector2  operator*( const cVector2& _other ) { return cVector2( x() * _other.x(), y() * _other.y() ); }
        cVector2& operator*=( const T& _value );
        cVector2& operator*=( const cVector2& _other );

        cVector2  operator/( const T& _value ) { return cVector2( x() / _value, y() / _value ); }
        cVector2  operator/( const cVector2& _other ) { return cVector2( x() / _other.x(), y() / _other.y() ); }
        cVector2& operator/=( const T& _value );
        cVector2& operator/=( const cVector2& _other );

        T&       operator[]( const size_t _index ) { return m_array[ _index ]; }
        const T& operator[]( const size_t _index ) const { return m_array[ _index ]; }

        T length() { return std::sqrt( std::pow( x(), 2 ) + std::pow( y(), 2 ) ); }
        T squaredLength() { return std::pow( x(), 2 ) + std::pow( y(), 2 ); }

        cVector2& normalize();
        cVector2  normalized();

        T        dot( const cVector2& _other ) { return x() * _other.x() + y() * _other.y(); }
        cVector2 project( const cVector2& _other ) { return _other * ( dot( _other ) / _other.dot( this ) ); }

        T angleBetween( const cVector2& _other ) { return std::acos( dot( _other ) / ( length() * _other.length() ) ); }
        T angleBetweenNormalized( const cVector2& _other ) { return std::acos( dot( _other ) ); }

        T&       x() { return m_array[ 0 ]; }
        const T& x() const { return m_array[ 0 ]; }
        T&       y() { return m_array[ 1 ]; }
        const T& y() const { return m_array[ 1 ]; }

    private:
        T m_array[ 2 ];
    };

    template< typename T >
    cVector2< T >::cVector2()
    : m_array{ 0, 0 }
    {}

    template< typename T >
    cVector2< T >::cVector2( const T& _value )
    : m_array{ _value, _value }
    {}

    template< typename T >
    cVector2< T >::cVector2( const T& _x, const T& _y )
    : m_array{ _x, _y }
    {}

    template< typename T >
    cVector2< T >::cVector2( const cVector2& _other )
    : m_array{ _other.x, _other.y }
    {}

    template< typename T >
    cVector2< T >& cVector2< T >::operator=( const T& _value )
    {
        x() = _value;
        y() = _value;
        return *this;
    }

    template< typename T >
    cVector2< T >& cVector2< T >::operator=( const cVector2& _other )
    {
        if( this != &_other )
        {
            x() = _other.x();
            y() = _other.y();
        }
        return *this;
    }

    template< typename T >
    cVector2< T >& cVector2< T >::operator+=( const T& _value )
    {
        x() += _value;
        y() += _value;
        return *this;
    }

    template< typename T >
    cVector2< T >& cVector2< T >::operator+=( const cVector2& _other )
    {
        if( this != &_other )
        {
            x() += _other.x();
            y() += _other.y();
        }
        return *this;
    }

    template< typename T >
    cVector2< T >& cVector2< T >::operator-=( const T& _value )
    {
        x() -= _value;
        y() -= _value;
        return *this;
    }

    template< typename T >
    cVector2< T >& cVector2< T >::operator-=( const cVector2& _other )
    {
        if( this != &_other )
        {
            x() -= _other.x();
            y() -= _other.y();
        }
        return *this;
    }

    template< typename T >
    cVector2< T >& cVector2< T >::operator*=( const T& _value )
    {
        x() *= _value;
        y() *= _value;
        return *this;
    }

    template< typename T >
    cVector2< T >& cVector2< T >::operator*=( const cVector2& _other )
    {
        if( this != &_other )
        {
            x() *= _other.x();
            y() *= _other.y();
        }
        return *this;
    }

    template< typename T >
    cVector2< T >& cVector2< T >::operator/=( const T& _value )
    {
        x() /= _value;
        y() /= _value;
        return *this;
    }

    template< typename T >
    cVector2< T >& cVector2< T >::operator/=( const cVector2& _other )
    {
        if( this != &_other )
        {
            x() /= _other.x();
            y() /= _other.y();
        }
        return *this;
    }

    template< typename T >
    cVector2< T >& cVector2< T >::normalize()
    {
        double len = length();
        x() /= len;
        y() /= len;
        return *this;
    }

    template< typename T >
    cVector2< T > cVector2< T >::normalized()
    {
        double len = length();
        if( len != 0 )
            return cVector2( x() / len, y() / len );
        return *this;
    }

    typedef cVector2< float > cVector2f;
}
