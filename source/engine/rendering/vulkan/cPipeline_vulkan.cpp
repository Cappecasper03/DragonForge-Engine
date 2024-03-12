#include "cPipeline_vulkan.h"

#include <tracy/Tracy.hpp>
#include <vector>

#include "engine/filesystem/cFileSystem.h"
#include "engine/log/Log.h"
#include "misc/Helper_vulkan.h"

namespace df::vulkan
{
	void cPipeline_vulkan::sCreateInfo::setShaders( const VkShaderModule _vertex, const VkShaderModule _fragment )
	{
		ZoneScoped;

		shader_stages.clear();
		shader_stages.push_back( helper::init::pipelineShaderStageCreateInfo( VK_SHADER_STAGE_VERTEX_BIT, _vertex ) );
		shader_stages.push_back( helper::init::pipelineShaderStageCreateInfo( VK_SHADER_STAGE_FRAGMENT_BIT, _fragment ) );
	}

	void cPipeline_vulkan::sCreateInfo::setInputTopology( const VkPrimitiveTopology _topology, const bool _primitive_restart_enable )
	{
		ZoneScoped;

		input_assembly.topology               = _topology;
		input_assembly.primitiveRestartEnable = _primitive_restart_enable;
	}

	void cPipeline_vulkan::sCreateInfo::setpolygonMode( const VkPolygonMode _mode, const float _line_width )
	{
		ZoneScoped;

		rasterizer.polygonMode = _mode;
		rasterizer.lineWidth   = _line_width;
	}

	void cPipeline_vulkan::sCreateInfo::setCullMode( const VkCullModeFlags _cull_mode, const VkFrontFace _front_face )
	{
		ZoneScoped;

		rasterizer.cullMode  = _cull_mode;
		rasterizer.frontFace = _front_face;
	}

	void cPipeline_vulkan::sCreateInfo::setMultisamplingNone()
	{
		ZoneScoped;

		multisampling.sampleShadingEnable   = false;
		multisampling.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading      = 1.0f;
		multisampling.pSampleMask           = nullptr;
		multisampling.alphaToCoverageEnable = false;
		multisampling.alphaToOneEnable      = false;
	}

	void cPipeline_vulkan::sCreateInfo::setDepthFormat( const VkFormat _format )
	{
		ZoneScoped;

		render_info.depthAttachmentFormat = _format;
	}

	void cPipeline_vulkan::sCreateInfo::disableBlending()
	{
		ZoneScoped;

		color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		color_blend_attachment.blendEnable    = false;
	}

	void cPipeline_vulkan::sCreateInfo::disableDepthtest()
	{
		ZoneScoped;

		depth_stencil.depthTestEnable       = false;
		depth_stencil.depthWriteEnable      = false;
		depth_stencil.depthCompareOp        = VK_COMPARE_OP_NEVER;
		depth_stencil.depthBoundsTestEnable = false;
		depth_stencil.stencilTestEnable     = false;
		depth_stencil.front                 = {};
		depth_stencil.back                  = {};
		depth_stencil.minDepthBounds        = 0;
		depth_stencil.maxDepthBounds        = 1;
	}

	cPipeline_vulkan::cPipeline_vulkan( const sCreateInfo& _create_info )
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

	bool cPipeline_vulkan::recreateGraphicsPipeline( const sCreateInfo& _create_info )
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

	bool cPipeline_vulkan::createGraphicsPipeline( const sCreateInfo& _create_info )
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
