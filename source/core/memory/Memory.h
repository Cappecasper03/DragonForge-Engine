#pragma once
#include <map>

#include "core/misc/iSingleton.h"

namespace vg::memory
{
    extern float getUsageKb();
    extern float getUsagePeakKb();

    extern void tTrack( void* _address, const size_t _size );

    template< typename T, typename... Params >
    T* tAlloc( const unsigned _amount, Params... _params )
    {
        T* address;

        if( _amount > 1 )
            address = new T[ _amount ]( _params... );
        else
            address = new T( _params... );

#if defined( DEBUG )
        tTrack( address, sizeof( T ) * _amount );
#endif

        return address;
    }

    extern void tFree( void* _address );
}

#define MEMORY_TRACK( _address, _size )     vg::memory::tTrack( _address, _size )
#define MEMORY_ALLOC( _type, _amount, ... ) vg::memory::tAlloc<_type>( _amount,  __VA_ARGS__ )
#define MEMORY_FREE( _address )             vg::memory::tFree( _address )
