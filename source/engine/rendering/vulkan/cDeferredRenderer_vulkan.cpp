#include "cDeferredRenderer_vulkan.h"

#include <tracy/Tracy.hpp>

#include "assets/cQuad_vulkan.h"
#include "callbacks/DefaultQuadCB_vulkan.h"
#include "cFramebuffer_vulkan.h"
#include "descriptor/sDescriptorLayoutBuilder_vulkan.h"
#include "engine/rendering/cRenderCallback.h"
#include "misc/Helper_vulkan.h"
#include "pipeline/sPipelineCreateInfo_vulkan.h"

namespace df::vulkan
{
	cDeferredRenderer_vulkan::~cDeferredRenderer_vulkan()
	{
		ZoneScoped;

		delete m_deferred_screen_quad->render_callback;
		delete m_deferred_screen_quad;
		m_texture_layout.reset();
	}

	void cDeferredRenderer_vulkan::initializeDeferred()
	{
		ZoneScoped;

		m_deferred_screen_quad = new cQuad_vulkan( "deferred", glm::vec3( m_window_size / 2, 0 ), glm::vec2( m_window_size ) );

		createQuadRenderCallback();
		createMeshRenderCallback();

		m_deferred_framebuffer = new cFramebuffer_vulkan( "deferred", 3, m_frames_in_flight, m_window_size );
	}

	void cDeferredRenderer_vulkan::createQuadRenderCallback()
	{
		ZoneScoped;

		sPipelineCreateInfo_vulkan pipeline_create_info{};

		pipeline_create_info.vertex_input_binding.emplace_back( 0, static_cast< uint32_t >( sizeof( cQuad_vulkan::sVertex ) ), vk::VertexInputRate::eVertex );

		pipeline_create_info.vertex_input_attribute.emplace_back( 0,
		                                                          0,
		                                                          vk::Format::eR32G32B32Sfloat,
		                                                          static_cast< uint32_t >( offsetof( cQuad_vulkan::sVertex, cQuad_vulkan::sVertex::position ) ) );
		pipeline_create_info.vertex_input_attribute.emplace_back( 1,
		                                                          0,
		                                                          vk::Format::eR32G32Sfloat,
		                                                          static_cast< uint32_t >( offsetof( cQuad_vulkan::sVertex, cQuad_vulkan::sVertex::tex_coord ) ) );

		pipeline_create_info.push_constant_ranges.emplace_back( vk::ShaderStageFlagBits::eVertex, 0, static_cast< uint32_t >( sizeof( sDeferredPushConstants ) ) );

		sDescriptorLayoutBuilder_vulkan descriptor_layout_builder{};
		descriptor_layout_builder.addBinding( 0, vk::DescriptorType::eCombinedImageSampler );
		descriptor_layout_builder.addBinding( 1, vk::DescriptorType::eCombinedImageSampler );
		descriptor_layout_builder.addBinding( 2, vk::DescriptorType::eCombinedImageSampler );
		m_texture_layout = descriptor_layout_builder.build( vk::ShaderStageFlagBits::eFragment );

		pipeline_create_info.descriptor_layouts.push_back( getVertexSceneUniformLayout() );
		pipeline_create_info.descriptor_layouts.push_back( m_texture_layout.get() );

		pipeline_create_info.setShaders( helper::util::createShaderModule( "default_quad_deferred_vertex" ), helper::util::createShaderModule( "default_quad_deferred_fragment" ) );
		pipeline_create_info.setInputTopology( vk::PrimitiveTopology::eTriangleList );
		pipeline_create_info.setpolygonMode( vk::PolygonMode::eFill );
		pipeline_create_info.setCullMode( vk::CullModeFlagBits::eNone, vk::FrontFace::eClockwise );
		pipeline_create_info.setColorFormat( getRenderColorFormat() );
		pipeline_create_info.setDepthFormat( getRenderDepthFormat() );
		pipeline_create_info.setMultisamplingNone();
		pipeline_create_info.disableDepthtest();
		pipeline_create_info.disableBlending();

		m_deferred_screen_quad->render_callback = new cRenderCallback( "default_quad_deferred", pipeline_create_info, render_callback::defaultQuadDeferred );
	}

	void cDeferredRenderer_vulkan::createMeshRenderCallback()
	{
		ZoneScoped;
	}
}