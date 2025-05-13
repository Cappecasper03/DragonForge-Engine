#pragma once

#include <vulkan/vulkan.hpp>

#include "cPipelineCreateInfo_vulkan.h"
#include "engine/core/utils/Misc.h"

namespace df::vulkan
{
	class cPipeline_vulkan
	{
	public:
		DF_DisableCopyAndMove( cPipeline_vulkan );

		explicit cPipeline_vulkan( const cPipelineCreateInfo_vulkan& _create_info );

		void recreateGraphicsPipeline( const cPipelineCreateInfo_vulkan& _create_info );

		vk::UniquePipeline       m_pipeline;
		vk::UniquePipelineLayout m_layout;

		std::string_view getName() const { return m_name; }

	private:
		void createGraphicsPipeline( const cPipelineCreateInfo_vulkan& _create_info );

		std::string m_name;
	};
}
