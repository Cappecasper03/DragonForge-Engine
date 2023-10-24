#include "Memory.h"

#include <cstdlib>
#include <format>

namespace df::memory
{
    struct sMemory
    {
        void*       address  = nullptr;
        size_t      size     = 0;
        std::string file     = {};
        std::string function = {};
        size_t      line     = 0;
    };

    std::unordered_map< size_t, sMemory > s_addresses   = {};
    size_t                                s_usage       = 0;
    size_t                                s_usage_peak  = 0;
    size_t                                s_allocations = 0;
    size_t                                s_frees       = 0;

    float getUsageMB() { return static_cast< float >( s_usage ) * pow( 10.f, -6.f ); }
    float getUsagePeakMB() { return static_cast< float >( s_usage_peak ) * pow( 10.f, -6.f ); }
    float getUsageKB() { return static_cast< float >( s_usage ) * pow( 10.f, -3.f ); }
    float getUsagePeakKB() { return static_cast< float >( s_usage_peak ) * pow( 10.f, -3.f ); }
    float getUsageB() { return static_cast< float >( s_usage ); }
    float getUsagePeakB() { return static_cast< float >( s_usage_peak ); }

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
        s_allocations++;

        if( s_usage > s_usage_peak )
            s_usage_peak = s_usage;

        s_addresses[ reinterpret_cast< size_t >( _address ) ] = memory;

        LOG_MEMORY( std::format( "[ALLOC] File: {:25} Function: {:10} Line: {:4} - Address: {:13} Size: {}", _file.substr( _file.find_last_of( '\\' ) + 1 ), _function, _line, _address, _size ) );
#endif
    }

    void tFree( void* _address, const std::string& _file, const char* _function, const unsigned _line )
    {
#if defined( DEBUG )
        const size_t   hash   = reinterpret_cast< size_t >( _address );
        const sMemory& memory = s_addresses[ hash ];

        LOG_MEMORY( std::format( "[FREE]  File: {:25} Function: {:10} Line: {:4} - Address: {:13} Size: {}", _file.substr( _file.find_last_of( '\\' ) + 1 ), _function, _line, _address, memory.size ) );

        s_frees++;
        s_usage -= memory.size;
        s_addresses.erase( hash );
#endif

        free( _address );
    }

    void printLeaks()
    {
        LOG_MEMORY( "" );
        LOG_MEMORY( "" );
        LOG_MEMORY( std::format( "Allocations: {}", s_allocations ) );
        LOG_MEMORY( std::format( "Frees:       {}", s_frees ) );
        LOG_MEMORY( std::format( "Leaks:       {}", s_addresses.size() ) );
        LOG_MEMORY( "" );

        size_t length_megabyte = std::format( "{}", getUsagePeakMB() ).length();
        size_t length_kilobyte = std::format( "{}", getUsagePeakKB() ).length();
        size_t length_byte     = std::format( "{}", getUsagePeakB() ).length();

        LOG_MEMORY( std::format( "Allocated memory: {:<{}} MB, {:<{}} KB, {:<{}} B",
                       getUsagePeakMB(), length_megabyte,
                       getUsagePeakKB(), length_kilobyte,
                       getUsagePeakB(), length_byte ) );

        LOG_MEMORY( std::format( "Freed memory:     {:<{}} MB, {:<{}} KB, {:<{}} B",
                       getUsagePeakMB() - getUsageMB(), length_megabyte,
                       getUsagePeakKB() - getUsageKB(), length_kilobyte,
                       getUsagePeakB() - getUsageB(), length_byte ) );

        LOG_MEMORY( std::format( "Leaked memory:    {:<{}} MB, {:<{}} KB, {:<{}} B",
                       getUsageMB(), length_megabyte,
                       getUsageKB(), length_kilobyte,
                       getUsageB(), length_byte ) );
    }
};
