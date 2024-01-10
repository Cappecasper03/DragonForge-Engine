#pragma once

#include <functional>
#include <ranges>

#include "iEvent.h"
#include "core/misc/Misc.h"
#include "core/profiling/Profiling.h"

namespace df
{
    template< typename... Targs >
    class cEvent final : public iEvent
    {
    public:
        DISABLE_COPY_AND_MOVE( cEvent );

        cEvent()           = default;
        ~cEvent() override = default;

        template< typename T >
        void subscribe( T* _object, void ( T::*_function )( Targs... ) );
        template< typename T >
        void subscribe( T* _object, void ( *_function )( Targs... ) );

        void unsubscribe( void* _object ) override;

        void invoke( Targs... _args );

    private:
        std::unordered_map< void*, std::function< void( Targs... ) > > m_subscribers;
    };

    template< typename... Targs >
    template< typename T >
    void cEvent< Targs... >::subscribe( T* _object, void ( T::*_function )( Targs... ) )
    {
#if defined( PROFILING )
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        m_subscribers[ _object ] = [_object, _function]( Targs... _args ) { ( _object->*_function )( _args... ); };
    }

    template< typename... Targs >
    template< typename T >
    void cEvent< Targs... >::subscribe( T* _object, void ( *_function )( Targs... ) )
    {
#if defined( PROFILING )
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        m_subscribers[ _object ] = [_function]( Targs... _args ) { ( *_function )( _args... ); };
    }

    template< typename... Targs >
    void cEvent< Targs... >::unsubscribe( void* _object )
    {
#if defined( PROFILING )
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        m_subscribers.erase( _object );
    }

    template< typename... Targs >
    void cEvent< Targs... >::invoke( Targs... _args )
    {
#if defined( PROFILING )
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        for( std::function< void( Targs... ) >& function : m_subscribers | std::ranges::views::values )
            function( _args... );
    }
}
