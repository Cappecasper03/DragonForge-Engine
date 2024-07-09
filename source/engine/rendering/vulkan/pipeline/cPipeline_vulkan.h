#pragma once

#include <vulkan/vulkan.hpp>

#include "engine/misc/Misc.h"
#include "sPipelineCreateInfo_vulkan.h"

namespace df::vulkan
{
	class cPipeline_vulkan
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( cPipeline_vulkan )

		explicit cPipeline_vulkan( const sPipelineCreateInfo_vulkan& _create_info );

		bool recreateGraphicsPipeline( const sPipelineCreateInfo_vulkan& _create_info );

		vk::UniquePipeline       pipeline;
		vk::UniquePipelineLayout layout;

	private:
		bool createGraphicsPipeline( const sPipelineCreateInfo_vulkan& _create_info );
	};
}
