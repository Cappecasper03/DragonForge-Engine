#pragma once

#include <vulkan/vulkan_core.h>

#include "engine/misc/Misc.h"
#include "sPipelineCreateInfo.h"

namespace df::vulkan
{
	class cPipeline_vulkan
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( cPipeline_vulkan )

		explicit cPipeline_vulkan( const sPipelineCreateInfo& _create_info );
		~cPipeline_vulkan();

		bool recreateGraphicsPipeline( const sPipelineCreateInfo& _create_info );

		VkPipeline       pipeline;
		VkPipelineLayout layout;

	private:
		bool createGraphicsPipeline( const sPipelineCreateInfo& _create_info );

		VkDevice m_logical_device;
	};
}
