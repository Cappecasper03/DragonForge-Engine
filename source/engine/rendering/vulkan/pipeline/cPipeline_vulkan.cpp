#include "cPipeline_vulkan.h"

#include <tracy/Tracy.hpp>
#include <vector>

#include "engine/filesystem/cFileSystem.h"
#include "engine/log/Log.h"
#include "engine/rendering/cRenderer.h"
#include "engine/rendering/vulkan/cRenderer_vulkan.h"

namespace df::vulkan
{
	cPipeline_vulkan::cPipeline_vulkan( const sPipelineCreateInfo_vulkan& _create_info )
	{
		ZoneScoped;

		createGraphicsPipeline( _create_info );
	}

	cPipeline_vulkan::~cPipeline_vulkan()
	{
		ZoneScoped;

		const cRenderer_vulkan* renderer       = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );
		const vk::UniqueDevice& logical_device = renderer->getLogicalDevice();

		logical_device->destroyPipeline( pipeline.get() );
		logical_device->destroyPipelineLayout( layout.get() );
	}

	bool cPipeline_vulkan::recreateGraphicsPipeline( const sPipelineCreateInfo_vulkan& _create_info )
	{
		ZoneScoped;

		const cRenderer_vulkan* renderer       = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );
		const vk::UniqueDevice& logical_device = renderer->getLogicalDevice();

		if( logical_device->waitIdle() != vk::Result::eSuccess )
			DF_LOG_ERROR( "Failed to wait for device idle" );

		logical_device->destroyPipeline( pipeline.get() );
		logical_device->destroyPipelineLayout( layout.get() );

		if( !createGraphicsPipeline( _create_info ) )
		{
			DF_LOG_ERROR( "Failed to recreate graphics pipeline" );
			return false;
		}

		DF_LOG_MESSAGE( "Recreated graphics pipeline" );
		return true;
	}

	bool cPipeline_vulkan::createGraphicsPipeline( const sPipelineCreateInfo_vulkan& _create_info )
	{
		ZoneScoped;

		const cRenderer_vulkan* renderer       = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );
		const vk::UniqueDevice& logical_device = renderer->getLogicalDevice();

		const std::vector                        dynamic_states = { vk::DynamicState::eScissor, vk::DynamicState::eViewport };
		const vk::PipelineDynamicStateCreateInfo dynamic_state_create_info( vk::PipelineDynamicStateCreateFlags(), dynamic_states );

		const vk::PipelineVertexInputStateCreateInfo vertex_input_create_info( vk::PipelineVertexInputStateCreateFlags(),
		                                                                       _create_info.vertex_input_binding,
		                                                                       _create_info.vertex_input_attribute );

		constexpr vk::PipelineViewportStateCreateInfo viewport_state_create_info( vk::PipelineViewportStateCreateFlags(), 1, nullptr, 1 );

		const vk::PipelineColorBlendStateCreateInfo color_blend_create_info( vk::PipelineColorBlendStateCreateFlags(),
		                                                                     false,
		                                                                     vk::LogicOp::eCopy,
		                                                                     1,
		                                                                     &_create_info.color_blend_attachment );

		const vk::PipelineLayoutCreateInfo pipeline_layout_create_info( vk::PipelineLayoutCreateFlags(), _create_info.descriptor_layouts, _create_info.push_constant_ranges );

		layout = logical_device->createPipelineLayoutUnique( pipeline_layout_create_info ).value;

		vk::GraphicsPipelineCreateInfo create_info( vk::PipelineCreateFlags(),
		                                            _create_info.shader_stages,
		                                            &vertex_input_create_info,
		                                            &_create_info.input_assembly,
		                                            nullptr,
		                                            &viewport_state_create_info,
		                                            &_create_info.rasterizer,
		                                            &_create_info.multisampling,
		                                            &_create_info.depth_stencil,
		                                            &color_blend_create_info,
		                                            &dynamic_state_create_info,
		                                            layout.get() );
		create_info.setPNext( &_create_info.render_info );

		pipeline = logical_device->createGraphicsPipelineUnique( vk::PipelineCache(), create_info ).value;

		for( const vk::PipelineShaderStageCreateInfo& shader_stage: _create_info.shader_stages )
			logical_device->destroyShaderModule( shader_stage.module );

		DF_LOG_MESSAGE( "Created graphics pipeline" );
		return true;
	}
}
