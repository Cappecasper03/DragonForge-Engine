#pragma once

#include "graphics/vulkan/assets/cQuad_vulkan.h"
#include "graphics/vulkan/pipeline/cPipeline_vulkan.h"

namespace df::vulkan::render_callbacks
{
	class cDefaultQuad_vulkan
	{
	public:
		static void forwardQuad( const cPipeline_vulkan* _pipeline, const cQuad_vulkan* _quad );

		static void deferredQuad( const cPipeline_vulkan* _pipeline, const cQuad_vulkan* _quad );
		static void deferredQuadFinal( const cPipeline_vulkan* _pipeline, const cQuad_vulkan* _quad );
	};
}
