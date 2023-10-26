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

    extern void alloc( void* _address, const size_t _size, const std::string& _file, const unsigned _line );

    template< typename T, typename... Targs >
    T* tAlloc( const unsigned _amount, const std::string& _file, const unsigned _line, Targs... _args )
    {
        T* address;

        if( _amount > 1 )
            address = static_cast< T* >( std::malloc( _amount * sizeof( T ) ) );
        else
            address = new T( _args... );

        memory::alloc( address, sizeof( T ) * _amount, _file, _line );

        return address;
    }

    extern void free( void* _address, const std::string& _file, const unsigned _line );

    template< typename T >
    extern void tFree( T* _address, const std::string& _file, const unsigned _line )
    {
        memory::free( _address, _file, _line );

        delete _address;
    }

    extern void printLeaks();
}

#define MEMORY_ALLOC( _type, _amount, ... ) df::memory::tAlloc< _type >( _amount, __FILE__, __LINE__, __VA_ARGS__ )
#define MEMORY_FREE( _address )             df::memory::tFree( _address, __FILE__, __LINE__ )
