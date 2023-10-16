#pragma once
#include <map>

#include "core/misc/iSingleton.h"

namespace vg::memory
{
    extern float getUsageKb();
    extern float getUsagePeakKb();

    extern void tTrack( void* _address, const size_t _size, const std::string& _file, const char* _function, const unsigned _line );

    template< typename T, typename... Params >
    T* tAlloc( const unsigned _amount, const std::string& _file, const char* _function, const unsigned _line, Params... _params )
    {
        T* address;

        if( _amount > 1 )
            address = new T[ _amount ]( _params... );
        else
            address = new T( _params... );

        tTrack( address, sizeof( T ) * _amount, _file, _function, _line );

        return address;
    }

    extern void tFree( void* _address, const std::string& _file, const char* _function, const unsigned _line );

    extern void print();
}

#define MEMORY_TRACK( _address, _size )     vg::memory::tTrack( _address, _size, __FILE__, __FUNCTION__, __LINE__, )
#define MEMORY_ALLOC( _type, _amount, ... ) vg::memory::tAlloc<_type>( _amount, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__ )
#define MEMORY_FREE( _address )             vg::memory::tFree( _address, __FILE__, __FUNCTION__, __LINE__ )
