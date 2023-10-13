#include "Memory.h"

#include <cstdlib>

namespace vg::memory
{
    std::map< size_t, cMemoryTracker::sMemory > cMemoryTracker::s_memory_adresses = {};
    size_t                                      cMemoryTracker::s_usage           = 0;
    size_t                                      cMemoryTracker::s_usage_peak      = 0;

    float cMemoryTracker::getUsageKb() { return static_cast< float >( s_usage ) / 1000; }
    float cMemoryTracker::getUsagePeakKb() { return static_cast< float >( s_usage_peak ); }

    void cMemoryTracker::tTrack( void* _address, const size_t _size )
    {
        sMemory memory;
        memory.address = _address;
        memory.size    = _size;

        s_usage += memory.size;

        if( s_usage > s_usage_peak )
            s_usage_peak = s_usage;

        s_memory_adresses[ reinterpret_cast< size_t >( _address ) ] = memory;
    }

    void cMemoryTracker::tFree( void* _address )
    {
        const size_t hash = reinterpret_cast< size_t >( _address );

        s_usage -= s_memory_adresses[ hash ].size;
        s_memory_adresses.erase( hash );
        free( _address );
    }
};
