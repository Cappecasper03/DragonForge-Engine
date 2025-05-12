#pragma once

#include <vulkan/vulkan.hpp>

#include "engine/profiling/ProfilingMacros_vulkan.h"
#include "engine/graphics/vulkan/cCommandBuffer.h"

namespace df::vulkan
{
	class cRenderer_vulkan;

	struct sSubmitContext_vulkan
	{
		void create();
		void create( const cRenderer_vulkan* _renderer );
		void destroy();

		vk::UniqueFence       fence;
		vk::UniqueCommandPool command_pool;
		cCommandBuffer        command_buffer;

		cProfilingContext tracy_context;
	};
}
