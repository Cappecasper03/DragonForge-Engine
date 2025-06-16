#pragma once

#include <vulkan/vulkan.hpp>

#include "engine/profiling/ProfilingMacros_vulkan.h"
#include "engine/graphics/vulkan/cCommandBuffer.h"

namespace df::vulkan
{
	class cGraphicsApi_vulkan;

	struct sSubmitContext_vulkan
	{
		void create();
		void create( const cGraphicsApi_vulkan* _graphics_api );
		void destroy();

		vk::UniqueFence       fence;
		vk::UniqueCommandPool command_pool;
		cCommandBuffer        command_buffer;

		cProfilingContext tracy_context;
	};
}
