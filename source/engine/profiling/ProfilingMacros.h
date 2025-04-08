#pragma once

#ifdef DF_Profiling
	#include <tracy/Tracy.hpp>

	#define DF_ProfilingScopeCPU                       ZoneScoped
	#define DF_ProfilingMessageColor( message, color ) TracyMessageC( message.data(), message.size(), color )
	#define DF_ProfilingFrameMark                      FrameMark
#else
	#define DF_ProfilingScopeCPU
	#define DF_ProfilingMessageColor( message, color )
	#define DF_ProfilingFrameMark
#endif