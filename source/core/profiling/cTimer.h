#pragma once

#include <chrono>

class cTimer
{
public:
    cTimer();

    void update() { m_last_update = std::chrono::high_resolution_clock::now(); }

    long long getSecondDelta( const bool _update ) { return getNanoDelta( _update ) / 1'000'000'000; }
    long long getMilliDelta( const bool _update ) { return getNanoDelta( _update ) / 1'000'000; }
    long long getMicroDelta( const bool _update ) { return getNanoDelta( _update ) / 1'000; }
    long long getNanoDelta( const bool _update );

    long long getSecondLife() const { return getNanoLife() / 1'000'000'000; }
    long long getMilliLife() const { return getNanoLife() / 1'000'000; }
    long long getMicroLife() const { return getNanoLife() / 1'000; }
    long long getNanoLife() const;

protected:
    std::chrono::high_resolution_clock::time_point m_start;
    std::chrono::high_resolution_clock::time_point m_last_update;
};
