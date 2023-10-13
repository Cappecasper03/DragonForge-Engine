#pragma once
#include <map>

#include "core/misc/iSingleton.h"

namespace vg::memory
{
    class cMemoryTracker final
    {
    public:
        static float getUsageKb();
        static float getUsagePeakKb();

        static void tTrack( void* _address, const size_t _size );

        template< typename T, typename... Params >
        static T* tAlloc( const unsigned _amount, Params... _params )
        {
            T* address = nullptr;

            if( _amount > 1 )
                address = new T[ _amount ]( _params... );
            else
                address = new T( _params... );

#if defined( DEBUG )
            tTrack( address, sizeof( T ) * _amount );
#endif

            return address;
        }

        static void tFree( void* _address );

    private:
        struct sMemory
        {
            void*  address = nullptr;
            size_t size    = 0;
        };

        static std::map< size_t, sMemory > s_memory_adresses;

        static size_t s_usage;
        static size_t s_usage_peak;
    };
}

#define MEMORY_TRACK( _address, _size )     vg::memory::cMemoryTracker::tTrack( _address, _size )
#define MEMORY_ALLOC( _type, _amount, ... ) vg::memory::cMemoryTracker::tAlloc<_type>( _amount,  __VA_ARGS__ )
#define MEMORY_FREE( _address )             vg::memory::cMemoryTracker::tFree( _address )
