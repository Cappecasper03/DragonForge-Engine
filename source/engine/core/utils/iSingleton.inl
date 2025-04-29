#pragma once

#include <cassert>

#include "core/Log.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	template< typename T >
	T* iSingleton< T >::s_instance = nullptr;

	template< typename T >
	template< typename... Targs >
	T* iSingleton< T >::initialize( Targs... _args )
	{
		DF_ProfilingScopeCpu;

		if( s_instance )
		{
			DF_LogError( "Singleton already initialized" );
			assert( !s_instance );
		}

		s_instance = new T( _args... );

		DF_LogMessage( "Initialized singleton" );
		return s_instance;
	}

	template< typename T >
	void iSingleton< T >::deinitialize()
	{
		DF_ProfilingScopeCpu;

		if( !s_instance )
		{
			DF_LogError( "No singleton initialized" );
			assert( s_instance );
		}

		delete s_instance;
		s_instance = nullptr;

		DF_LogMessage( "Deinitialized singleton" );
	}
}
