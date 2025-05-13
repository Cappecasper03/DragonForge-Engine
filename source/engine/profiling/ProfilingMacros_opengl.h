#pragma once

#ifdef DF_Profiling
	#include <tracy/TracyOpenGL.hpp>

	#define DF_ProfilingGpuContext                      TracyGpuContext
	#define DF_ProfilingScopeGpu                        TracyGpuZone( __FUNCTION__ )
	#define DF_ProfilingScopeNamedGpu( var_name, name ) TracyGpuNamedZone( var_name, name, true )
	#define DF_ProfilingCollectGpu                      TracyGpuCollect
#else
	#define DF_ProfilingGpuContext
	#define DF_ProfilingScopeGpu
	#define DF_ProfilingScopeNamedGpu( var_name, name )
	#define DF_ProfilingCollectGpu
#endif