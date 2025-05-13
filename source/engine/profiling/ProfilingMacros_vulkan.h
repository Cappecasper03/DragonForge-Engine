#pragma once

#ifdef DF_Profiling
	#include <tracy/TracyVulkan.hpp>

typedef TracyVkCtx cProfilingContext;

	#define DF_ProfilingScopeGpu( context, command_buffer )            TracyVkZone( context, command_buffer, __FUNCTION__ )
	#define DF_ProfilingScopeNamedGpu( context, command_buffer, name ) TracyVkZone( context, command_buffer, name )
	#define DF_ProfilingCollectGpu( context, command_buffer )          TracyVkCollect( context, command_buffer )
	#define DF_DestroyProfilingContext( context )                      TracyVkDestroy( context )
#else
typedef void* cProfilingContext;

	#define DF_ProfilingScopeGpu( context, command_buffer )
	#define DF_ProfilingScopeNamedGpu( context, command_buffer, name )
	#define DF_ProfilingCollectGpu( context, command_buffer )
	#define DF_DestroyProfilingContext( context )
#endif