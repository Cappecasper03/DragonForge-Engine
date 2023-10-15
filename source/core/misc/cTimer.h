#pragma once

#include <chrono>

namespace vg
{
    class cTimer
    {
    public:
        cTimer();

        void update() { m_last_update = std::chrono::high_resolution_clock::now(); }

        double getSecondDelta( const bool _update ) { return getNanoDelta( _update ) / 1'000'000'000; }
        double getMilliDelta( const bool _update ) { return getNanoDelta( _update ) / 1'000'000; }
        double getMicroDelta( const bool _update ) { return getNanoDelta( _update ) / 1'000; }
        double getNanoDelta( const bool _update );

        double getSecondLife() const { return getNanoLife() / 1'000'000'000; }
        double getMilliLife() const { return getNanoLife() / 1'000'000; }
        double getMicroLife() const { return getNanoLife() / 1'000; }
        double getNanoLife() const;

    protected:
        std::chrono::high_resolution_clock::time_point m_start;
        std::chrono::high_resolution_clock::time_point m_last_update;
    };
}
