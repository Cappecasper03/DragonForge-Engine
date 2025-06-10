#pragma once

#include "engine/core/utils/Misc.h"
#include "engine/graphics/vulkan/types/sAllocatedBuffer_vulkan.h"

namespace df::vulkan
{
	struct sRenderAsset_vulkan
	{
		DF_DeleteCopyAndMove( sRenderAsset_vulkan );

		explicit sRenderAsset_vulkan() = default;
		virtual ~sRenderAsset_vulkan();

		sAllocatedBuffer_vulkan m_vertex_buffer;
		sAllocatedBuffer_vulkan m_index_buffer;
	};
}
