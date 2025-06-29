#pragma once

#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	template< typename... Targs >
	template< typename T >
	void cEvent< Targs... >::subscribe( T* _object, void ( T::*_function )( Targs... ) )
	{
		DF_ProfilingScopeCpu;

		m_function = [ _object, _function ]( Targs... _args ) { ( _object->*_function )( _args... ); };
	}

	template< typename... Targs >
	template< typename T >
	void cEvent< Targs... >::subscribe( void ( *_function )( Targs... ) )
	{
		DF_ProfilingScopeCpu;

		m_function = [ _function ]( Targs... _args ) { ( *_function )( _args... ); };
	}
}
