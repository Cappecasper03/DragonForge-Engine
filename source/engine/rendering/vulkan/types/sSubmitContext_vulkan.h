#pragma once

#include <vulkan/vulkan.hpp>

#include "engine/profiling/ProfilingMacros_vulkan.h"

namespace df::vulkan
{
	class cRenderer_vulkan;

	struct sSubmitContext_vulkan
	{
		void create();
		void create( const cRenderer_vulkan* _renderer );
		void destroy();

		vk::UniqueFence         fence;
		vk::UniqueCommandPool   command_pool;
		vk::UniqueCommandBuffer command_buffer;

		#ifdef DF_Profiling
		TracyVkCtx tracy_context;
		#endif
	};
}
