#pragma once

#ifdef DF_Profiling
	#include <tracy/TracyOpenGL.hpp>

	#define DF_ProfilingGPUContext                      TracyGpuContext
	#define DF_ProfilingScopeGPU                        TracyGpuZone( __FUNCTION__ )
	#define DF_ProfilingScopeNamedGPU( var_name, name ) TracyGpuNamedZone( var_name, name )
	#define DF_ProfilingCollectGPU                      TracyGpuCollect
#else
	#define DF_ProfilingGPUContext
	#define DF_ProfilingScopeGPU
	#define DF_ProfilingScopeNamedGPU( var_name, name )
	#define DF_ProfilingCollectGPU
#endif