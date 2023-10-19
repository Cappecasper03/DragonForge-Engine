#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "core/misc/iSingleton.h"

namespace df
{
    namespace event
    {
        static std::string input     = "input";
        static std::string update    = "update";
        static std::string render_3d = "render3D";
        static std::string render_2d = "render2D";
    }

    class cEventManager final : public iSingleton< cEventManager >
    {
    public:
        DISABLE_COPY_AND_MOVE( cEventManager );

        cEventManager()           = default;
        ~cEventManager() override = default;

        template< typename T, typename... Targs >
        static void subscribe( const std::string& _name, T* _object, void ( T::*_function )( Targs... ) );
        static void unsubscribe( const std::string& _name, void* _object );

        template< typename... Targs >
        static void invoke( const std::string& _name, Targs... _args );

    private:
        std::vector< std::function< void() > > m_events;
    };

    template< typename T, typename... Targs >
    void cEventManager::subscribe( const std::string& /*_name*/, T* _object, void ( T::*_function )( Targs... ) )
    {
        getInstance()->m_events.push_back( [_object, _function]( Targs... _args ) { ( _object->*_function )( _args... ); } );
    }

    template< typename... Targs >
    void cEventManager::invoke( const std::string& /*_name*/, Targs... _args )
    {
        for( std::function< void() >& event : getInstance()->m_events )
            event( _args... );
    }
}
