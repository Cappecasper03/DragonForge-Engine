#include "iSampler.h"

#include "engine/graphics/cRenderer.h"
#include "engine/graphics/opengl/assets/textures/cSampler_opengl.h"
#include "engine/graphics/vulkan/assets/textures/cSampler_vulkan.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{

	iSampler* iSampler::create()
	{
		DF_ProfilingScopeCpu;

		switch( cRenderer::getDeviceType() )
		{
			case cRenderer::kOpenGl: return new opengl::cSampler_opengl();
			case cRenderer::kVulkan: return new vulkan::cSampler_vulkan();
		}

		return nullptr;
	}
}