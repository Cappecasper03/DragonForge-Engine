#pragma once

#include <vulkan/vulkan.hpp>

#include "engine/profiling/ProfilingMacros_vulkan.h"
#include "engine/graphics/vulkan/cCommandBuffer.h"

namespace df::vulkan
{
	class cGraphicsDevice_vulkan;

	struct sSubmitContext_vulkan
	{
		void create();
		void create( const cGraphicsDevice_vulkan* _renderer );
		void destroy();

		vk::UniqueFence       fence;
		vk::UniqueCommandPool command_pool;
		cCommandBuffer        command_buffer;

		cProfilingContext tracy_context;
	};
}
