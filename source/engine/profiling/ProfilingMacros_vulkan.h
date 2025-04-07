#pragma once

#ifdef DF_Profiling
#include <tracy/TracyVulkan.hpp>

typedef tracy::VkCtx* cProfilingContext

#define DF_ProfilingScopeGPU( context, command_buffer ) TracyVkZone( context, command_buffer, __FUNCTION__ )
#define DF_DestroyProfilingContext( context ) TracyVkDestroy( context )
#else
typedef void* cProfilingContext

#define DF_ProfilingScopeGPU
#define DF_DestroyProfilingContext
#endif