#include "Memory.h"

#include <cstdlib>

namespace vg::memory
{
    struct sMemory
    {
        void*       address  = nullptr;
        size_t      size     = 0;
        std::string file     = {};
        std::string function = {};
        size_t      line     = 0;
    };

    std::map< size_t, sMemory > s_memory_adresses = {};
    size_t                      s_usage           = 0;
    size_t                      s_usage_peak      = 0;

    float getUsageKb() { return static_cast< float >( s_usage ) / 1000; }
    float getUsagePeakKb() { return static_cast< float >( s_usage_peak ); }

    void tTrack( void* _address, const size_t _size, const std::string& _file, const char* _function, const unsigned _line )
    {
        sMemory memory;
        memory.address  = _address;
        memory.size     = _size;
        memory.file     = _file;
        memory.function = _function;
        memory.line     = _line;

        s_usage += memory.size;

        if( s_usage > s_usage_peak )
            s_usage_peak = s_usage;

        s_memory_adresses[ reinterpret_cast< size_t >( _address ) ] = memory;
    }

    void tFree( void* _address )
    {
        const size_t hash = reinterpret_cast< size_t >( _address );

        s_usage -= s_memory_adresses[ hash ].size;
        s_memory_adresses.erase( hash );
        free( _address );
    }
};
