#pragma once

#ifdef DF_Profiling
	#include <tracy/Tracy.hpp>

	#define DF_ProfilingScopeCpu                       ZoneScoped
	#define DF_ProfilingScopeNamesCpu( name )          ZoneScopedN( name )
	#define DF_ProfilingMessageColor( message, color ) TracyMessageC( ( message ).data(), ( message ).size(), color )
	#define DF_ProfilingFrameMark                      FrameMark
#else
	#define DF_ProfilingScopeCpu
	#define DF_ProfilingScopeNamesCpu
	#define DF_ProfilingMessageColor( message, color )
	#define DF_ProfilingFrameMark
#endif