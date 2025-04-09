#pragma once

#include <ranges>

#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	template< typename... Targs >
	template< typename T >
	void cEvent< Targs... >::subscribe( T* _object, void ( T::*_function )( Targs... ) )
	{
		DF_ProfilingScopeCpu;

		m_subscribers[ _object ] = [ _object, _function ]( Targs... _args ) { ( _object->*_function )( _args... ); };
	}

	template< typename... Targs >
	template< typename T >
	void cEvent< Targs... >::subscribe( T* _object, void ( *_function )( Targs... ) )
	{
		DF_ProfilingScopeCpu;

		m_subscribers[ _object ] = [ _function ]( Targs... _args ) { ( *_function )( _args... ); };
	}

	template< typename... Targs >
	void cEvent< Targs... >::unsubscribe( void* _object )
	{
		DF_ProfilingScopeCpu;

		m_subscribers.erase( _object );
	}

	template< typename... Targs >
	void cEvent< Targs... >::invoke( Targs... _args )
	{
		DF_ProfilingScopeCpu;

		for( std::function< void( Targs... ) >& function: m_subscribers | std::ranges::views::values )
			function( _args... );
	}
}
