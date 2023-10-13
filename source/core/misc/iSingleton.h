#pragma once

#include <assert.h>

#include "Misc.h"
#include "core/log/Log.h"
#include "core/memory/Memory.h"

namespace vg
{
    template< typename T >
    class iSingleton
    {
    public:
        DISABLE_COPY_AND_MOVE( iSingleton )

        iSingleton() { s_instance = reinterpret_cast< T* >( this ); }
        virtual ~iSingleton() { s_instance = nullptr; }

        template< typename... Params >
        static void initialize( Params... _params )
        {
            assert( "Singleton already initialized" && !s_instance );

            s_instance = new T( _params... );
            TRACK( s_instance, sizeof( T ) );

            LOG_MESSAGE( "Initializing singleton" );
        }

        static void deinitialize()
        {
            assert( "No singleton initialized" && s_instance );

            FREE( s_instance );
            s_instance = nullptr;

            LOG_MESSAGE( "Deinitializing singleton" );
        }

        static T* getInstance() { return s_instance; }

    private:
        static T* s_instance;
    };

    template< typename T >
    T* iSingleton< T >::s_instance = nullptr;
}
