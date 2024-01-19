#pragma once

#include "Misc.h"
#include "core/log/Log.h"

#if PROFILING
#include "core/profiling/Profiling.h"
#endif

namespace df
{
    template< typename T >
    class iSingleton
    {
    public:
        DISABLE_COPY_AND_MOVE( iSingleton )

        iSingleton() { s_instance = reinterpret_cast< T* >( this ); }
        virtual ~iSingleton() { s_instance = nullptr; }

        template< typename... Targs >
        static T* initialize( Targs... _args );

        static void deinitialize();

        static T* getInstance() { return s_instance; }

    private:
        static T* s_instance;
    };

    template< typename T >
    template< typename... Targs >
    T* iSingleton< T >::initialize( Targs... _args )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        if( s_instance )
        {
            LOG_ERROR( "Singleton already initialized" );
            _ASSERT( !s_instance );
        }

        s_instance = new T( _args... );

        LOG_MESSAGE( "Initialized singleton" );
        return s_instance;
    }

    template< typename T >
    void iSingleton< T >::deinitialize()
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        if( !s_instance )
        {
            LOG_ERROR( "No singleton initialized" );
            _ASSERT( s_instance );
        }

        delete s_instance;
        s_instance = nullptr;

        LOG_MESSAGE( "Deinitialized singleton" );
    }

    template< typename T >
    T* iSingleton< T >::s_instance = nullptr;
}
