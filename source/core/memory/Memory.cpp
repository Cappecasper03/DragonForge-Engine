#include "Memory.h"

#include <cstdlib>
#include <map>
#include <vector>
#include <string>

namespace Memory
{
    struct sMemory
    {
        void*       address  = nullptr;
        size_t      size     = 0;
        const char* file     = nullptr;
        int         line     = 0;
        const char* function = nullptr;
    };

    std::map< size_t, sMemory > memory_adresses;

    size_t usage     = 0;
    size_t max_usage = 0;

    float getUsageKb() { return static_cast< float >( usage ) / 1000; }
    float getMaxUsageKb() { return static_cast< float >( max_usage ); }
};

void* operator new( const size_t _size, const char* _file, const int _line, const char* _function )
{
    void* address = malloc( _size );
    Memory::usage += _size;

    if( Memory::usage > Memory::max_usage )
        Memory::max_usage = Memory::usage;

    Memory::sMemory memory;
    memory.address  = address;
    memory.size     = _size;
    memory.file     = _file;
    memory.line     = _line;
    memory.function = _function;

    constexpr std::hash< std::string > string_hash;
    constexpr std::hash< int >         int_hash;
    const size_t                       file_hash( string_hash( std::string( _file ) ) );
    const size_t                       function_hash( string_hash( std::string( _function ) ) );
    const size_t                       line_hash( int_hash( _line ) );

    const size_t hash               = file_hash + function_hash * line_hash;
    Memory::memory_adresses[ hash ] = memory;

    return address;
}

void operator delete( void* _address, const char* _file, const int _line, const char* _function )
{
    constexpr std::hash< std::string > string_hash;
    constexpr std::hash< int >         int_hash;
    const size_t                       file_hash( string_hash( std::string( _file ) ) );
    const size_t                       function_hash( string_hash( std::string( _function ) ) );
    const size_t                       line_hash( int_hash( _line ) );
    const size_t                       hash = file_hash + function_hash * line_hash;

    Memory::usage -= Memory::memory_adresses[ hash ].size;
    Memory::memory_adresses.erase( hash );
    free( _address );
}
