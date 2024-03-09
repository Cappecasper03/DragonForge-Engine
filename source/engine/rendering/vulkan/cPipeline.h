#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

#include "engine/misc/Misc.h"

namespace df::vulkan
{
	class cPipeline
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( cPipeline )

		struct sCreateInfo
		{
			VkDevice                                         logical_device = nullptr;
			VkRenderPass                                     render_pass    = nullptr;
			std::vector< VkPipelineShaderStageCreateInfo >   shader_stages_create_info;
			std::vector< VkVertexInputBindingDescription >   vertex_input_binding_descriptions;
			std::vector< VkVertexInputAttributeDescription > vertex_input_attribute_descriptions;
			std::vector< VkPushConstantRange >               push_constant_ranges;
		};

		explicit cPipeline( const sCreateInfo& _create_info );
		~cPipeline();

		bool recreateGraphicsPipeline( const sCreateInfo& _create_info );

		VkPipeline       pipeline;
		VkPipelineLayout layout;

	private:
		bool createGraphicsPipeline( const sCreateInfo& _create_info );

		VkDevice m_logical_device;
	};
}
