#include "cPipeline.h"

#include <format>
#include <tracy/Tracy.hpp>
#include <vector>

#include "engine/filesystem/cFileSystem.h"
#include "engine/log/Log.h"

namespace df::vulkan
{
	cPipeline::cPipeline( const sCreateInfo& _create_info )
		: m_logical_device( _create_info.logical_device )
	{
		ZoneScoped;

		createGraphicsPipeline( _create_info );
	}

	cPipeline::~cPipeline()
	{
		ZoneScoped;

		vkDestroyPipeline( m_logical_device, pipeline, nullptr );
	}

	bool cPipeline::recreateGraphicsPipeline( const sCreateInfo& _create_info )
	{
		ZoneScoped;

		vkDeviceWaitIdle( m_logical_device );

		vkDestroyPipeline( m_logical_device, pipeline, nullptr );
		vkDestroyPipelineLayout( m_logical_device, layout, nullptr );

		if( m_logical_device != _create_info.logical_device )
			m_logical_device = _create_info.logical_device;

		if( !createGraphicsPipeline( _create_info ) )
		{
			DF_LOG_ERROR( "Failed to recreate graphics pipeline" );
			return false;
		}

		DF_LOG_MESSAGE( "Recreated graphics pipeline" );
		return true;
	}

	bool cPipeline::createGraphicsPipeline( const sCreateInfo& _create_info )
	{
		ZoneScoped;

		const std::vector                dynamic_states = { VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_VIEWPORT };
		VkPipelineDynamicStateCreateInfo dynamic_state_create_info{};
		dynamic_state_create_info.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamic_state_create_info.dynamicStateCount = static_cast< uint32_t >( dynamic_states.size() );
		dynamic_state_create_info.pDynamicStates    = dynamic_states.data();

		VkPipelineVertexInputStateCreateInfo vertex_input_create_info{};
		vertex_input_create_info.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertex_input_create_info.vertexBindingDescriptionCount   = static_cast< uint32_t >( _create_info.vertex_input_binding_descriptions.size() );
		vertex_input_create_info.pVertexBindingDescriptions      = _create_info.vertex_input_binding_descriptions.data();
		vertex_input_create_info.vertexAttributeDescriptionCount = static_cast< uint32_t >( _create_info.vertex_input_attribute_descriptions.size() );
		vertex_input_create_info.pVertexAttributeDescriptions    = _create_info.vertex_input_attribute_descriptions.data();

		VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info{};
		input_assembly_create_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		input_assembly_create_info.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		input_assembly_create_info.primitiveRestartEnable = VK_FALSE;

		VkPipelineViewportStateCreateInfo viewport_state_create_info{};
		viewport_state_create_info.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewport_state_create_info.viewportCount = 1;
		viewport_state_create_info.scissorCount  = 1;

		VkPipelineRasterizationStateCreateInfo rasterization_create_info{};
		rasterization_create_info.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterization_create_info.depthClampEnable        = VK_FALSE;
		rasterization_create_info.rasterizerDiscardEnable = VK_FALSE;
		rasterization_create_info.polygonMode             = VK_POLYGON_MODE_FILL;
		rasterization_create_info.lineWidth               = 1;
		rasterization_create_info.cullMode                = VK_CULL_MODE_BACK_BIT;
		rasterization_create_info.frontFace               = VK_FRONT_FACE_CLOCKWISE;
		rasterization_create_info.depthBiasEnable         = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo multisample_create_info{};
		multisample_create_info.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisample_create_info.sampleShadingEnable  = VK_FALSE;
		multisample_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineColorBlendAttachmentState color_blend_attachment{};
		color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		color_blend_attachment.blendEnable    = VK_FALSE;

		VkPipelineColorBlendStateCreateInfo color_blend_create_info{};
		color_blend_create_info.sType               = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		color_blend_create_info.logicOpEnable       = VK_FALSE;
		color_blend_create_info.logicOp             = VK_LOGIC_OP_COPY;
		color_blend_create_info.attachmentCount     = 1;
		color_blend_create_info.pAttachments        = &color_blend_attachment;
		color_blend_create_info.blendConstants[ 0 ] = 0;
		color_blend_create_info.blendConstants[ 1 ] = 0;
		color_blend_create_info.blendConstants[ 2 ] = 0;
		color_blend_create_info.blendConstants[ 3 ] = 0;

		VkPipelineLayoutCreateInfo pipeline_layout_create_info{};
		pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

		if( vkCreatePipelineLayout( m_logical_device, &pipeline_layout_create_info, nullptr, &layout ) != VK_SUCCESS )
		{
			DF_LOG_ERROR( "Failed to create graphics pipeline" );
			return false;
		}

		VkGraphicsPipelineCreateInfo create_info{};
		create_info.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		create_info.stageCount          = static_cast< uint32_t >( _create_info.shader_stages_create_info.size() );
		create_info.pStages             = _create_info.shader_stages_create_info.data();
		create_info.pVertexInputState   = &vertex_input_create_info;
		create_info.pInputAssemblyState = &input_assembly_create_info;
		create_info.pViewportState      = &viewport_state_create_info;
		create_info.pRasterizationState = &rasterization_create_info;
		create_info.pMultisampleState   = &multisample_create_info;
		create_info.pColorBlendState    = &color_blend_create_info;
		create_info.pDynamicState       = &dynamic_state_create_info;
		create_info.layout              = layout;
		create_info.renderPass          = _create_info.render_pass;
		create_info.subpass             = 0;
		create_info.basePipelineHandle  = nullptr;

		if( vkCreateGraphicsPipelines( m_logical_device, VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline ) != VK_SUCCESS )
		{
			DF_LOG_ERROR( "Failed to create graphics pipeline" );
			return false;
		}

		DF_LOG_MESSAGE( "Created graphics pipeline" );
		return true;
	}
}
