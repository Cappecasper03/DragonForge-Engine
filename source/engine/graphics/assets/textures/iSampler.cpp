#include "iSampler.h"

#include "engine/graphics/cRenderer.h"
#include "engine/graphics/opengl/assets/textures/cSampler_opengl.h"
#include "engine/graphics/vulkan/assets/textures/cSampler_vulkan.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{

	cUnique< iSampler > iSampler::create()
	{
		DF_ProfilingScopeCpu;

		switch( cRenderer::getApiType() )
		{
			case cRenderer::kOpenGl: return MakeUnique< opengl::cSampler_opengl >();
			case cRenderer::kVulkan: return MakeUnique< vulkan::cSampler_vulkan >();
		}

		return nullptr;
	}
}