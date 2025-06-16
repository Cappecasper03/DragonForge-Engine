#pragma once

#include <string>
#include <unordered_map>

#include "engine/events/cEvent.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	template< typename T, typename... Targs >
	void cEventManager::subscribe( const std::string& _name, T* _object, void ( T::*_function )( Targs... ) )
	{
		DF_ProfilingScopeCpu;

		cUnique< iEvent >& event = getInstance()->m_events[ _name ];

		if( !event )
			event = MakeUnique< cEvent< Targs... > >();

		reinterpret_cast< cEvent< Targs... >* >( event.get() )->subscribe( _object, _function );
	}

	template< typename T, typename... Targs >
	void cEventManager::subscribe( const std::string& _name, T* _object, void ( *_function )( Targs... ) )
	{
		DF_ProfilingScopeCpu;

		cUnique< iEvent >& event = getInstance()->m_events[ _name ];

		if( !event )
			event = MakeUnique< cEvent< Targs... > >();

		reinterpret_cast< cEvent< Targs... >* >( event.get() )->subscribe( _object, _function );
	}

	template< typename T >
	void cEventManager::unsubscribe( const std::string& _name, T* _object )
	{
		DF_ProfilingScopeCpu;

		const cUnique< iEvent >& event = getInstance()->m_events[ _name ];

		if( event )
			event->unsubscribe( _object );
	}

	template< typename... Targs >
	void cEventManager::invoke( const std::string& _name, Targs... _args )
	{
		DF_ProfilingScopeCpu;

		cEvent< Targs... >* event = reinterpret_cast< cEvent< Targs... >* >( getInstance()->m_events[ _name ].get() );

		if( event )
			event->invoke( _args... );
	}
}
