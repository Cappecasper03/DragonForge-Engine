#pragma once

#include <ranges>
#include <string>
#include <unordered_map>

#include "engine/events/cEvent.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	inline cEventManager::~cEventManager()
	{
		DF_ProfilingScopeCPU;

		for( const iEvent* event: m_events | std::ranges::views::values )
			delete event;
	}

	template< typename T, typename... Targs >
	void cEventManager::subscribe( const std::string& _name, T* _object, void ( T::*_function )( Targs... ) )
	{
		DF_ProfilingScopeCPU;

		auto event = reinterpret_cast< cEvent< Targs... >* >( getInstance()->m_events[ _name ] );

		if( !event )
		{
			event                            = new cEvent< Targs... >;
			getInstance()->m_events[ _name ] = event;
		}

		event->subscribe( _object, _function );
	}

	template< typename T, typename... Targs >
	void cEventManager::subscribe( const std::string& _name, T* _object, void ( *_function )( Targs... ) )
	{
		DF_ProfilingScopeCPU;

		auto event = reinterpret_cast< cEvent< Targs... >* >( getInstance()->m_events[ _name ] );

		if( !event )
		{
			event                            = new cEvent< Targs... >();
			getInstance()->m_events[ _name ] = event;
		}

		event->subscribe( _object, _function );
	}

	template< typename T >
	void cEventManager::unsubscribe( const std::string& _name, T* _object )
	{
		DF_ProfilingScopeCPU;

		const auto event = getInstance()->m_events[ _name ];

		if( event )
			event->unsubscribe( _object );
	}

	template< typename... Targs >
	void cEventManager::invoke( const std::string& _name, Targs... _args )
	{
		DF_ProfilingScopeCPU;

		auto event = reinterpret_cast< cEvent< Targs... >* >( getInstance()->m_events[ _name ] );
		if( event )
			event->invoke( _args... );
	}
}
