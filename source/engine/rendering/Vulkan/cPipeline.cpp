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
		VkPipelineDynamicStateCreateInfo dynamic_state_create_info{
			.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			.dynamicStateCount = static_cast< uint32_t >( dynamic_states.size() ),
			.pDynamicStates    = dynamic_states.data(),
		};

		VkPipelineVertexInputStateCreateInfo vertex_input_create_info{
			.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			.vertexBindingDescriptionCount   = static_cast< uint32_t >( _create_info.vertex_input_binding_descriptions.size() ),
			.pVertexBindingDescriptions      = _create_info.vertex_input_binding_descriptions.data(),
			.vertexAttributeDescriptionCount = static_cast< uint32_t >( _create_info.vertex_input_attribute_descriptions.size() ),
			.pVertexAttributeDescriptions    = _create_info.vertex_input_attribute_descriptions.data(),
		};

		VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info{
			.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			.primitiveRestartEnable = VK_FALSE,
		};

		VkPipelineViewportStateCreateInfo viewport_state_create_info{
			.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			.viewportCount = 1,
			.scissorCount  = 1,
		};

		VkPipelineRasterizationStateCreateInfo rasterization_create_info{
			.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			.depthClampEnable        = VK_FALSE,
			.rasterizerDiscardEnable = VK_FALSE,
			.polygonMode             = VK_POLYGON_MODE_FILL,
			.cullMode                = VK_CULL_MODE_BACK_BIT,
			.frontFace               = VK_FRONT_FACE_CLOCKWISE,
			.depthBiasEnable         = VK_FALSE,
			.lineWidth               = 1,
		};

		VkPipelineMultisampleStateCreateInfo multisample_create_info{
			.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
			.sampleShadingEnable  = VK_FALSE,
		};

		VkPipelineColorBlendAttachmentState color_blend_attachment{
			.blendEnable    = VK_FALSE,
			.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
		};

		VkPipelineColorBlendStateCreateInfo color_blend_create_info{
			.sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			.logicOpEnable   = VK_FALSE,
			.logicOp         = VK_LOGIC_OP_COPY,
			.attachmentCount = 1,
			.pAttachments    = &color_blend_attachment,
			.blendConstants  = {0, 0, 0, 0},
		};

		VkPipelineLayoutCreateInfo pipeline_layout_create_info{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		};

		if( vkCreatePipelineLayout( m_logical_device, &pipeline_layout_create_info, nullptr, &layout ) != VK_SUCCESS )
		{
			DF_LOG_ERROR( "Failed to create graphics pipeline" );
			return false;
		}

		VkGraphicsPipelineCreateInfo create_info{
			.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.stageCount          = static_cast< uint32_t >( _create_info.shader_stages_create_info.size() ),
			.pStages             = _create_info.shader_stages_create_info.data(),
			.pVertexInputState   = &vertex_input_create_info,
			.pInputAssemblyState = &input_assembly_create_info,
			.pViewportState      = &viewport_state_create_info,
			.pRasterizationState = &rasterization_create_info,
			.pMultisampleState   = &multisample_create_info,
			.pColorBlendState    = &color_blend_create_info,
			.pDynamicState       = &dynamic_state_create_info,
			.layout              = layout,
			.renderPass          = _create_info.render_pass,
			.subpass             = 0,
			.basePipelineHandle  = nullptr,
		};

		if( vkCreateGraphicsPipelines( m_logical_device, VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline ) != VK_SUCCESS )
		{
			DF_LOG_ERROR( "Failed to create graphics pipeline" );
			return false;
		}

		for( const VkPipelineShaderStageCreateInfo& shader_stage: _create_info.shader_stages_create_info )
			vkDestroyShaderModule( m_logical_device, shader_stage.module, nullptr );

		DF_LOG_MESSAGE( "Created graphics pipeline" );
		return true;
	}
}
