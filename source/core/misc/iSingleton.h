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
            if( s_instance )
            {
                LOG_ERROR( "Singleton already initialized" );
                _ASSERT( !s_instance );
            }

            s_instance = new T( _params... );

            LOG_MESSAGE( "Initialized singleton" );
        }

        static void deinitialize()
        {
            if( !s_instance )
            {
                LOG_ERROR( "No singleton initialized" );
                _ASSERT( s_instance );
            }

            s_instance = nullptr;

            LOG_MESSAGE( "Deinitialized singleton" );
        }

        static T* getInstance() { return s_instance; }

    private:
        static T* s_instance;
    };

    template< typename T >
    T* iSingleton< T >::s_instance = nullptr;
}
