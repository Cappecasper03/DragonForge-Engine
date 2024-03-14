#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>

namespace df::vulkan
{
	struct sPipelineCreateInfo
	{
		void setShaders( VkShaderModule _vertex, VkShaderModule _fragment );
		void setInputTopology( VkPrimitiveTopology _topology, bool _primitive_restart_enable = false );
		void setpolygonMode( VkPolygonMode _mode, float _line_width = 1 );
		void setCullMode( VkCullModeFlags _cull_mode, VkFrontFace _front_face );
		void setMultisamplingNone();

		void enableBlendingAdditive();
		void enableBlendingAlphablend();
		void disableBlending();

		void enableDepthtest( bool _depth_write_enable, VkCompareOp _operation );
		void disableDepthtest();

		void setColorFormat( VkFormat _format );
		void setDepthFormat( VkFormat _format );

		VkDevice logical_device = nullptr;

		VkPipelineRasterizationStateCreateInfo rasterizer     = { .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };
		VkPipelineRenderingCreateInfo          render_info    = { .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO };
		VkPipelineDepthStencilStateCreateInfo  depth_stencil  = { .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };
		VkPipelineMultisampleStateCreateInfo   multisampling  = { .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };
		VkPipelineInputAssemblyStateCreateInfo input_assembly = { .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
		VkPipelineColorBlendAttachmentState    color_blend_attachment{};
		VkFormat                               color_attachment_format;

		std::vector< VkPipelineShaderStageCreateInfo >   shader_stages;
		std::vector< VkPushConstantRange >               push_constant_ranges;
		std::vector< VkVertexInputBindingDescription >   vertex_input_binding;
		std::vector< VkVertexInputAttributeDescription > vertex_input_attribute;
	};
}
