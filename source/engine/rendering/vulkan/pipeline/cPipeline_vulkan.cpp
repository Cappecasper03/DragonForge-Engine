#include "cPipeline_vulkan.h"

#include <tracy/Tracy.hpp>
#include <vector>

#include "engine/filesystem/cFileSystem.h"
#include "engine/log/Log.h"

namespace df::vulkan
{
	cPipeline_vulkan::cPipeline_vulkan( const sPipelineCreateInfo& _create_info )
		: m_logical_device( _create_info.logical_device )
	{
		ZoneScoped;

		createGraphicsPipeline( _create_info );
	}

	cPipeline_vulkan::~cPipeline_vulkan()
	{
		ZoneScoped;

		vkDestroyPipeline( m_logical_device, pipeline, nullptr );
		vkDestroyPipelineLayout( m_logical_device, layout, nullptr );
	}

	bool cPipeline_vulkan::recreateGraphicsPipeline( const sPipelineCreateInfo& _create_info )
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

	bool cPipeline_vulkan::createGraphicsPipeline( const sPipelineCreateInfo& _create_info )
	{
		ZoneScoped;

		const std::vector                dynamic_states = { VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_VIEWPORT };
		VkPipelineDynamicStateCreateInfo dynamic_state_create_info{
			.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			.pNext             = nullptr,
			.dynamicStateCount = static_cast< uint32_t >( dynamic_states.size() ),
			.pDynamicStates    = dynamic_states.data(),
		};

		VkPipelineVertexInputStateCreateInfo vertex_input_create_info{
			.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			.vertexBindingDescriptionCount   = static_cast< uint32_t >( _create_info.vertex_input_binding.size() ),
			.pVertexBindingDescriptions      = _create_info.vertex_input_binding.data(),
			.vertexAttributeDescriptionCount = static_cast< uint32_t >( _create_info.vertex_input_attribute.size() ),
			.pVertexAttributeDescriptions    = _create_info.vertex_input_attribute.data(),
		};

		VkPipelineViewportStateCreateInfo viewport_state_create_info{
			.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			.viewportCount = 1,
			.scissorCount  = 1,
		};

		VkPipelineColorBlendStateCreateInfo color_blend_create_info{
			.sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			.logicOpEnable   = false,
			.logicOp         = VK_LOGIC_OP_COPY,
			.attachmentCount = 1,
			.pAttachments    = &_create_info.color_blend_attachment,
		};

		const VkPipelineLayoutCreateInfo pipeline_layout_create_info{
			.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			.setLayoutCount         = static_cast< uint32_t >( _create_info.descriptor_layouts.size() ),
			.pSetLayouts            = _create_info.descriptor_layouts.data(),
			.pushConstantRangeCount = static_cast< uint32_t >( _create_info.push_constant_ranges.size() ),
			.pPushConstantRanges    = _create_info.push_constant_ranges.data(),
		};

		if( vkCreatePipelineLayout( m_logical_device, &pipeline_layout_create_info, nullptr, &layout ) != VK_SUCCESS )
		{
			DF_LOG_ERROR( "Failed to create graphics pipeline" );
			return false;
		}

		const VkGraphicsPipelineCreateInfo create_info{
			.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.pNext               = &_create_info.render_info,
			.stageCount          = static_cast< uint32_t >( _create_info.shader_stages.size() ),
			.pStages             = _create_info.shader_stages.data(),
			.pVertexInputState   = &vertex_input_create_info,
			.pInputAssemblyState = &_create_info.input_assembly,
			.pViewportState      = &viewport_state_create_info,
			.pRasterizationState = &_create_info.rasterizer,
			.pMultisampleState   = &_create_info.multisampling,
			.pDepthStencilState  = &_create_info.depth_stencil,
			.pColorBlendState    = &color_blend_create_info,
			.pDynamicState       = &dynamic_state_create_info,
			.layout              = layout,
			.subpass             = 0,
			.basePipelineHandle  = nullptr,
		};

		if( vkCreateGraphicsPipelines( m_logical_device, nullptr, 1, &create_info, nullptr, &pipeline ) != VK_SUCCESS )
		{
			DF_LOG_ERROR( "Failed to create graphics pipeline" );
			return false;
		}

		for( const VkPipelineShaderStageCreateInfo& shader_stage: _create_info.shader_stages )
			vkDestroyShaderModule( m_logical_device, shader_stage.module, nullptr );

		DF_LOG_MESSAGE( "Created graphics pipeline" );
		return true;
	}
}
