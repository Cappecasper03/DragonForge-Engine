#pragma once

#include <tracy/Tracy.hpp>

#include "engine/rendering/vulkan/assets/cQuad_vulkan.h"
#include "engine/rendering/vulkan/cPipeline_vulkan.h"

namespace df::vulkan::render_callback
{
	inline void defaultQuad( const cPipeline_vulkan* /*_pipeline*/, const cQuad_vulkan* /*_quad*/ )
	{
		ZoneScoped;

		// const cCamera* camera = cCameraManager::getInstance()->current;
	}

	inline void defaultQuadDeferred( const cPipeline_vulkan* /*_pipeline*/, const cQuad_vulkan* /*_quad*/ )
	{
		ZoneScoped;

		// const cFramebuffer* render_framebuffer = reinterpret_cast< const cFramebuffer* >( cRenderer::getRenderInstance()->getFramebuffer() );
		// const cCamera*      camera             = cCameraManager::getInstance()->current;
	}
}
