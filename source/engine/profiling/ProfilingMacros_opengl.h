#ifdef DF_Profiling
#include "engine/profiling/ProfilingMacros_opengl.h"

#define DF_ProfilingScopeGPU TracyGpuZone( __FUNCTION__ )
#else
#define DF_ProfilingScopeGPU
#endif