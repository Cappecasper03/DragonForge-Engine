#pragma once

#include <string>
#include <unordered_map>

#include "engine/events/cMultiEvent.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	template< typename T, typename... Targs >
	void cEventManager::subscribe( const std::string& _name, T* _object, void ( T::*_function )( Targs... ) )
	{
		DF_ProfilingScopeCpu;

		cUnique< iEvent >& event = getInstance()->m_events[ _name ];

		if( !event )
			event = MakeUnique< cMultiEvent< Targs... > >();

		reinterpret_cast< cMultiEvent< Targs... >* >( event.get() )->subscribe( _object, _function );
	}

	template< typename T, typename... Targs >
	void cEventManager::subscribe( const std::string& _name, T* _object, void ( *_function )( Targs... ) )
	{
		DF_ProfilingScopeCpu;

		cUnique< iEvent >& event = getInstance()->m_events[ _name ];

		if( !event )
			event = MakeUnique< cMultiEvent< Targs... > >();

		reinterpret_cast< cMultiEvent< Targs... >* >( event.get() )->subscribe( _object, _function );
	}

	template< typename T, typename... Targs >
	void cEventManager::subscribe( const std::string& _name, T* _object, std::function< void( Targs... ) > _function )
	{
		DF_ProfilingScopeCpu;

		cUnique< iEvent >& event = getInstance()->m_events[ _name ];

		if( !event )
			event = MakeUnique< cMultiEvent< Targs... > >();

		reinterpret_cast< cMultiEvent< Targs... >* >( event.get() )->subscribe( _object, _function );
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

		cMultiEvent< Targs... >* event = reinterpret_cast< cMultiEvent< Targs... >* >( getInstance()->m_events[ _name ].get() );

		if( event )
			event->invoke( _args... );
	}
}
