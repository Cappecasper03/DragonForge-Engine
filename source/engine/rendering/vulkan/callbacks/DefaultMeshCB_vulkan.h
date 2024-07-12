#pragma once

#include <tracy/Tracy.hpp>

#include "engine/rendering/vulkan/assets/cMesh_vulkan.h"
#include "engine/rendering/vulkan/pipeline/cPipeline_vulkan.h"

namespace df::vulkan::render_callback
{
	inline void defaultMeshAmbient( const cPipeline_vulkan* /*_pipeline*/, const cMesh_vulkan* /*_mesh*/ )
	{
		ZoneScoped;
	}
}
