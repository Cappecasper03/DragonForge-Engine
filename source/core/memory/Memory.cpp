#include "Memory.h"

#include <cstdlib>
#include <format>

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
#if defined( DEBUG )
        sMemory memory  = {};
        memory.address  = _address;
        memory.size     = _size;
        memory.file     = _file;
        memory.function = _function;
        memory.line     = _line;

        s_usage += memory.size;

        if( s_usage > s_usage_peak )
            s_usage_peak = s_usage;

        s_memory_adresses[ reinterpret_cast< size_t >( _address ) ] = memory;

        LOG_MEMORY( std::format( "[ALLOC] {:25} {:10} Line {:4} - {:13} Size {}", _file, _function, _line, _address, _size ) );
#endif
    }

    void tFree( void* _address, const std::string& _file, const char* _function, const unsigned _line )
    {
#if defined( DEBUG )
        const size_t   hash   = reinterpret_cast< size_t >( _address );
        const sMemory& memory = s_memory_adresses[ hash ];

        LOG_MEMORY( std::format( "[FREE]  {:25} {:15} Line {:4} - {:15} Size {}", _file, _function, _line, _address, memory.size ) );

        s_usage -= memory.size;
        s_memory_adresses.erase( hash );
#endif

        free( _address );
    }
};
