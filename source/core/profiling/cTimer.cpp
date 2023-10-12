#include "cTimer.h"

cTimer::cTimer()
: m_start( std::chrono::high_resolution_clock::now() )
, m_last_update( std::chrono::high_resolution_clock::now() )
{}

long long cTimer::getNanoDelta( const bool _update )
{
    const std::chrono::time_point< std::chrono::steady_clock > now        = std::chrono::high_resolution_clock::now();
    const long long                                            delta_time = std::chrono::duration_cast< std::chrono::nanoseconds >( now - m_last_update ).count();

    if( _update )
        m_last_update = now;

    return delta_time;
}

long long cTimer::getNanoLife() const
{
    const std::chrono::time_point< std::chrono::steady_clock > now = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast< std::chrono::nanoseconds >( now - m_start ).count();
}
