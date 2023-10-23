#pragma once

#include <unordered_map>

#include "core/misc/iSingleton.h"

namespace df::memory
{
    extern float getUsageMB();
    extern float getUsagePeakMB();
    extern float getUsageKB();
    extern float getUsagePeakKB();
    extern float getUsageB();
    extern float getUsagePeakB();

    extern void tTrack( void* _address, const size_t _size, const std::string& _file, const char* _function, const unsigned _line );

    template< typename T, typename... Targs >
    T* tAlloc( const unsigned _amount, const std::string& _file, const char* _function, const unsigned _line, Targs... _args )
    {
        T* address;

        if( _amount > 1 )
            address = static_cast< T* >( std::malloc( _amount * sizeof( T ) ) );
        else
            address = new T( _args... );

        tTrack( address, sizeof( T ) * _amount, _file, _function, _line );

        return address;
    }

    extern void tFree( void* _address, const std::string& _file, const char* _function, const unsigned _line );

    extern void printLeaks();
}

#define MEMORY_TRACK( _address, _size )     df::memory::tTrack( _address, _size, __FILE__, __FUNCTION__, __LINE__, )
#define MEMORY_ALLOC( _type, _amount, ... ) df::memory::tAlloc< _type >( _amount, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__ )
#define MEMORY_FREE( _address )             df::memory::tFree( _address, __FILE__, __FUNCTION__, __LINE__ )
