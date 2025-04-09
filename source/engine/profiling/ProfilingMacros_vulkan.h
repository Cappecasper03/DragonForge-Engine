#pragma once

#ifdef DF_Profiling
	#include <tracy/TracyVulkan.hpp>

typedef TracyVkCtx cProfilingContext;

	#define DF_ProfilingScopeGpu( context, command_buffer )   TracyVkZone( context, command_buffer, __FUNCTION__ )
	#define DF_DestroyProfilingContext( context )             TracyVkDestroy( context )
	#define DF_ProfilingCollectGpu( context, command_buffer ) TracyVkCollect( context, command_buffer )
#else
typedef void* cProfilingContext;

	#define DF_ProfilingScopeGpu( context, command_buffer )
	#define DF_DestroyProfilingContext( context )
	#define DF_ProfilingCollectGpu( context, command_buffer )
#endif