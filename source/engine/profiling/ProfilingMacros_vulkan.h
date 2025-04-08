#pragma once

#ifdef DF_Profiling
	#include <tracy/TracyVulkan.hpp>

typedef tracy::VkCtx* cProfilingContext

	#define DF_ProfilingScopeGPU( context, command_buffer )   TracyVkZone( context, command_buffer, __FUNCTION__ )
	#define DF_DestroyProfilingContext( context )             TracyVkDestroy( context )
	#define DF_ProfilingCollectGPU( context, command_buffer ) TracyVkCollect( context, command_buffer )
#else
typedef void* cProfilingContext

	#define DF_ProfilingScopeGPU( context, command_buffer )
	#define DF_DestroyProfilingContext( context )
	#define DF_ProfilingCollectGPU( context, command_buffer )
#endif