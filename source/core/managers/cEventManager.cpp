#include "cEventManager.h"

#include <ranges>

namespace df
{
    cEventManager::~cEventManager()
    {
        for( sEvent* event : m_events | std::views::values )
        {
            if( event )
                MEMORY_FREE( event );
        }
    }

    void cEventManager::subscribe( const std::string& _name, void* _object, std::function< void( void* ) >& _function )
    {
        sEvent* event = getInstance()->m_events[ _name ];

        if( !event )
        {
            event       = MEMORY_ALLOC( sEvent, 1 );
            event->name = _name;
        }

        event->subscribers[ _object ] = _function;
    }

    void cEventManager::unsubscribe( const std::string& _name, void* _object )
    {
        sEvent* event = getInstance()->m_events[ _name ];

        if( !event )
            return;

        event->subscribers.erase( _object );

        if( event->subscribers.empty() )
            getInstance()->m_events.erase( _name );
    }

    void cEventManager::invoke( const std::string& _name, void* _data )
    {
        sEvent* event = getInstance()->m_events[ _name ];

        if( !event )
            return;

        for( std::function< void( void* ) >& function : event->subscribers | std::views::values )
            function( _data );
    }
}
