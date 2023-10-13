#include "Memory.h"

#include <cstdlib>
#include <map>
#include <vector>
#include <string>

namespace vg::memory
{
    struct sMemory
    {
        void*       address  = nullptr;
        size_t      size     = 0;
        const char* file     = nullptr;
        int         line     = 0;
        const char* function = nullptr;
    };

    inline size_t generateUniqueHash( const char* _file, const int _line, const char* _function )
    {
        constexpr std::hash< std::string > string_hash;
        constexpr std::hash< int >         int_hash;
        const size_t                       file_hash( string_hash( std::string( _file ) ) );
        const size_t                       function_hash( string_hash( std::string( _function ) ) );
        const size_t                       line_hash( int_hash( _line ) );

        return file_hash + function_hash * line_hash + line_hash;
    }

    std::map< size_t, sMemory > memory_adresses;

    size_t usage      = 0;
    size_t usage_peak = 0;

    float getUsageKb() { return static_cast< float >( usage ) / 1000; }
    float getUsagePeakKb() { return static_cast< float >( usage_peak ); }

    void track( void* _address, const size_t _size, const char* _file, const int _line, const char* _function )
    {
        sMemory memory;
        memory.address  = _address;
        memory.size     = _size;
        memory.file     = _file;
        memory.line     = _line;
        memory.function = _function;

        usage += memory.size;

        if( usage > usage_peak )
            usage_peak = usage;

        const size_t hash       = generateUniqueHash( _file, _line, _function );
        memory_adresses[ hash ] = memory;
    }
};

void free( void* _address, const char* _file, const int _line, const char* _function )
{
    const size_t hash = vg::memory::generateUniqueHash( _file, _line, _function );

    vg::memory::usage -= vg::memory::memory_adresses[ hash ].size;
    vg::memory::memory_adresses.erase( hash );
    free( _address );
}
