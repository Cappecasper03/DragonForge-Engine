﻿#pragma once

#include "engine/misc/Misc.h"
#include "graphics/vulkan/types/sAllocatedBuffer_vulkan.h"

namespace df::vulkan
{
	struct sRenderAsset_vulkan
	{
		DF_DisableCopyAndMove( sRenderAsset_vulkan );

		explicit sRenderAsset_vulkan() = default;
		virtual ~sRenderAsset_vulkan();

		sAllocatedBuffer_vulkan vertex_buffer;
		sAllocatedBuffer_vulkan fragment_buffer;
		sAllocatedBuffer_vulkan index_buffer;
	};
}
