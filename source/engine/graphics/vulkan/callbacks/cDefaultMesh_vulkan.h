#pragma once

#include "engine/graphics/vulkan/assets/cMesh_vulkan.h"
#include "engine/graphics/vulkan/pipeline/cPipeline_vulkan.h"

namespace df::vulkan::render_callbacks
{
	class cDefaultMesh_vulkan
	{
	public:
		static void forwardMesh( const cPipeline_vulkan* _pipeline, const cMesh_vulkan* _mesh );

		static void deferredMesh( const cPipeline_vulkan* _pipeline, const cMesh_vulkan* _mesh );
	};
}
