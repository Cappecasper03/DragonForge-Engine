#pragma once

#include <numbers>

namespace df::math
{
    template< typename T >
    T radToDeg( const T& _value ) { return _value * ( T( 180 ) / std::numbers::pi_v< T > ); }

    template< typename T >
    T degToRad( const T& _value ) { return _value * ( std::numbers::pi_v< T > / T( 180 ) ); }
}
