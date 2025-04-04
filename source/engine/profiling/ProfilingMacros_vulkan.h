#ifdef DF_Profiling
#include <tracy/TracyVulkan.hpp>

#define DF_ProfilingScopeGPU( context, command_buffer ) TracyVkZone( context, command_buffer, __FUNCTION__ )
#else
#define DF_ProfilingScopeGPU
#endif