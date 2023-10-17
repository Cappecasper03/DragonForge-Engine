#pragma once

#include <numbers>

namespace df::math
{
    template< typename T >
    T radToDeg( const T& _value ) { return _value * ( T( 180 ) / std::numbers::pi_v< T > ); }

    template< typename T >
    T degToRad( const T& _value ) { return _value * ( std::numbers::pi_v< T > / T( 180 ) ); }

    template< typename T >
    T interpolate( const T& _current, const T& _target, const float& _delta_time, const float& _speed ) { return _current + ( _target - _current ) * _delta_time * _speed; }
}
