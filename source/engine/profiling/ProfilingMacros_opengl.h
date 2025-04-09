#pragma once

#ifdef DF_Profiling
	#include <tracy/TracyOpenGL.hpp>

	#define DF_ProfilingGPUContext                      TracyGpuContext
	#define DF_ProfilingScopeGpu                        TracyGpuZone( __FUNCTION__ )
	#define DF_ProfilingScopeNamedGPU( var_name, name ) TracyGpuNamedZone( var_name, name, true )
	#define DF_ProfilingCollectGpu                      TracyGpuCollect
#else
	#define DF_ProfilingGPUContext
	#define DF_ProfilingScopeGpu
	#define DF_ProfilingScopeNamedGPU( var_name, name )
	#define DF_ProfilingCollectGpu
#endif