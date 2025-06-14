﻿#include "cPipeline_vulkan.h"

#include <vector>

#include "engine/core/Log.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/vulkan/cGraphicsDevice_vulkan.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::vulkan
{
	cPipeline_vulkan::cPipeline_vulkan( const cPipelineCreateInfo_vulkan& _create_info )
	{
		DF_ProfilingScopeCpu;

		m_name = _create_info.m_name;
		createGraphicsPipeline( _create_info );
	}

	void cPipeline_vulkan::recreateGraphicsPipeline( const cPipelineCreateInfo_vulkan& _create_info )
	{
		DF_ProfilingScopeCpu;

		if( reinterpret_cast< cGraphicsDevice_vulkan* >( cRenderer::getGraphicsDevice() )->getLogicalDevice().waitIdle() != vk::Result::eSuccess )
			DF_LogError( "Failed to wait for device idle" );

		m_pipeline.reset();
		m_layout.reset();

		createGraphicsPipeline( _create_info );
		DF_LogMessage( "Recreated graphics pipeline" );
	}

	void cPipeline_vulkan::createGraphicsPipeline( const cPipelineCreateInfo_vulkan& _create_info )
	{
		DF_ProfilingScopeCpu;

		const cGraphicsDevice_vulkan* renderer       = reinterpret_cast< cGraphicsDevice_vulkan* >( cRenderer::getGraphicsDevice() );
		const vk::Device&       logical_device = renderer->getLogicalDevice();

		const std::vector                        dynamic_states = { vk::DynamicState::eScissor, vk::DynamicState::eViewport };
		const vk::PipelineDynamicStateCreateInfo dynamic_state_create_info( vk::PipelineDynamicStateCreateFlags(), dynamic_states );

		const vk::PipelineVertexInputStateCreateInfo vertex_input_create_info( vk::PipelineVertexInputStateCreateFlags(),
		                                                                       _create_info.m_vertex_input_binding,
		                                                                       _create_info.m_vertex_input_attribute );

		constexpr vk::PipelineViewportStateCreateInfo viewport_state_create_info( vk::PipelineViewportStateCreateFlags(), 1, nullptr, 1 );

		std::vector< vk::PipelineColorBlendAttachmentState > color_blend_attachment_states;
		for( uint32_t i = 0; i < _create_info.m_render_info.colorAttachmentCount; ++i )
			color_blend_attachment_states.push_back( _create_info.m_color_blend_attachment );

		const vk::PipelineColorBlendStateCreateInfo color_blend_create_info( vk::PipelineColorBlendStateCreateFlags(), false, vk::LogicOp::eCopy, color_blend_attachment_states );

		const vk::PipelineLayoutCreateInfo pipeline_layout_create_info( vk::PipelineLayoutCreateFlags(), _create_info.m_descriptor_layouts, _create_info.m_push_constant_ranges );

		m_layout = logical_device.createPipelineLayoutUnique( pipeline_layout_create_info ).value;

		vk::GraphicsPipelineCreateInfo create_info( vk::PipelineCreateFlags(),
		                                            _create_info.m_shader_stages,
		                                            &vertex_input_create_info,
		                                            &_create_info.m_input_assembly,
		                                            nullptr,
		                                            &viewport_state_create_info,
		                                            &_create_info.m_rasterizer,
		                                            &_create_info.m_multisampling,
		                                            &_create_info.m_depth_stencil,
		                                            &color_blend_create_info,
		                                            &dynamic_state_create_info,
		                                            m_layout.get() );
		create_info.setPNext( &_create_info.m_render_info );

		m_pipeline = logical_device.createGraphicsPipelineUnique( vk::PipelineCache(), create_info ).value;

		for( const vk::PipelineShaderStageCreateInfo& shader_stage: _create_info.m_shader_stages )
			logical_device.destroyShaderModule( shader_stage.module );

		DF_LogMessage( "Created graphics pipeline" );
	}
}
