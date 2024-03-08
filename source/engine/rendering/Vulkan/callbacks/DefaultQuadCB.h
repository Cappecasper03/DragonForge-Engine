#pragma once

#include <tracy/Tracy.hpp>

#include "engine/rendering/vulkan/cFramebuffer.h"
#include "engine/rendering/vulkan/cPipeline.h"

namespace df::vulkan::render_callback
{
	inline void defaultQuad( const cPipeline* /*_pipeline*/, const cQuad* /*_quad*/ )
	{
		ZoneScoped;

		// const cCamera* camera = cCameraManager::getInstance()->current;
	}

	inline void defaultQuadDeferred( const cPipeline* /*_pipeline*/, const cQuad* /*_quad*/ )
	{
		ZoneScoped;

		// const cFramebuffer* render_framebuffer = reinterpret_cast< const cFramebuffer* >( cRendererSingleton::getRenderInstance()->getFramebuffer() );
		// const cCamera*      camera             = cCameraManager::getInstance()->current;
	}
}
