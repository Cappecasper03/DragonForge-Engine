﻿#include "cDeferredRenderer_vulkan.h"

#include "assets/cQuad_vulkan.h"
#include "callbacks/cDefaultQuad_vulkan.h"
#include "cFramebuffer_vulkan.h"
#include "core/math/math.h"
#include "descriptor/sDescriptorLayoutBuilder_vulkan.h"
#include "engine/graphics/callback/cRenderCallback.h"
#include "engine/managers/cEventManager.h"
#include "engine/profiling/ProfilingMacros.h"
#include "engine/profiling/ProfilingMacros_vulkan.h"
#include "graphics/cameras/cCamera.h"
#include "graphics/window/iWindow.h"
#include "managers/assets/cCameraManager.h"
#include "misc/cTransform.h"
#include "pipeline/sPipelineCreateInfo_vulkan.h"
#include "types/Helper_vulkan.h"

namespace df::vulkan
{
	cDeferredRenderer_vulkan::cDeferredRenderer_vulkan( const std::string& _window_name )
		: cRenderer_vulkan( _window_name )
		, m_begin_deferred( true )
	{
		DF_ProfilingScopeCpu;
	}

	cDeferredRenderer_vulkan::~cDeferredRenderer_vulkan()
	{
		DF_ProfilingScopeCpu;

		delete m_deferred_framebuffer;
		delete m_deferred_screen_quad->render_callback;
		delete m_deferred_screen_quad;
		m_deferred_layout.reset();
	}

	void cDeferredRenderer_vulkan::beginRendering( const int _clear_buffers, const cColor& _color )
	{
		if( !m_begin_deferred )
		{
			cRenderer_vulkan::beginRendering( _clear_buffers, _color );
			return;
		}

		DF_ProfilingScopeCpu;
		const sFrameData_vulkan& frame_data = getCurrentFrame();
		DF_ProfilingScopeGpu( frame_data.profiling_context, frame_data.command_buffer.get() );

		const bool color = _clear_buffers & cCamera::eClearBuffer::kColor;
		const bool depth = _clear_buffers & cCamera::eClearBuffer::kDepth;

		const cCommandBuffer&    command_buffer = frame_data.command_buffer;
		const vk::ClearValue     clear_color_value( vk::ClearColorValue( _color.r, _color.g, _color.b, _color.a ) );
		constexpr vk::ClearValue clear_depth_stencil_value( vk::ClearDepthStencilValue( 1 ) );

		const cFramebuffer_vulkan*                   framebuffer        = reinterpret_cast< cFramebuffer_vulkan* >( m_deferred_framebuffer );
		const std::vector< sAllocatedImage_vulkan >& framebuffer_images = framebuffer->getCurrentFrameImages( getCurrentFrameIndex() );

		std::vector< vk::RenderingAttachmentInfo > color_attachments;
		color_attachments.reserve( framebuffer_images.size() );

		for( const sAllocatedImage_vulkan& framebuffer_image: framebuffer_images )
		{
			color_attachments.push_back(
				helper::init::attachmentInfo( framebuffer_image.image_view.get(), color ? &clear_color_value : nullptr, vk::ImageLayout::eColorAttachmentOptimal ) );
		}

		const vk::RenderingAttachmentInfo depth_attachment = helper::init::attachmentInfo( m_depth_image.image_view.get(),
		                                                                                   depth ? &clear_depth_stencil_value : nullptr,
		                                                                                   vk::ImageLayout::eDepthAttachmentOptimal );

		command_buffer.beginRendering( m_render_extent, color_attachments, &depth_attachment );
	}

	void cDeferredRenderer_vulkan::renderDeferred( const vk::CommandBuffer& _command_buffer )
	{
		DF_ProfilingScopeCpu;
#ifdef DF_Profiling
		const sFrameData_vulkan& frame_data = getCurrentFrame();
		DF_ProfilingScopeGpu( frame_data.profiling_context, frame_data.command_buffer.get() );
#endif

		const cFramebuffer_vulkan*                   framebuffer     = reinterpret_cast< cFramebuffer_vulkan* >( m_deferred_framebuffer );
		const std::vector< sAllocatedImage_vulkan >& deferred_images = framebuffer->getCurrentFrameImages( getCurrentFrameIndex() );

		for( const sAllocatedImage_vulkan& image: deferred_images )
			helper::util::transitionImage( _command_buffer, image.image.get(), vk::ImageLayout::eUndefined, vk::ImageLayout::eGeneral );

		m_begin_deferred = true;
		cEventManager::invoke( event::render_3d );
		cEventManager::invoke( event::render_2d );

		for( const sAllocatedImage_vulkan& image: deferred_images )
			helper::util::transitionImage( _command_buffer, image.image.get(), vk::ImageLayout::eUndefined, vk::ImageLayout::eShaderReadOnlyOptimal );

		m_begin_deferred = false;
		cCamera* camera  = cCameraManager::get( "default_2d" );
		camera->beginRender( cCamera::kDepth );
		m_deferred_screen_quad->render();
		camera->endRender();
	}

	void cDeferredRenderer_vulkan::initializeDeferred()
	{
		DF_ProfilingScopeCpu;

		m_deferred_screen_quad = new cQuad_vulkan( "deferred", cVector3f( m_window->getSize() / 2, 0 ), m_window->getSize() );
		cMatrix4f& transform   = m_deferred_screen_quad->transform->local;
		transform.rotate( math::radians( 180.f ), cVector3f( 0.f, 0.f, 1.f ) );
		transform.rotate( math::radians( 180.f ), cVector3f( 0.f, 1.f, 0.f ) );
		m_deferred_screen_quad->transform->update();

		createQuadRenderCallback();

		m_deferred_framebuffer = new cFramebuffer_vulkan( "deferred", 3, m_frames_in_flight, m_window->getSize() );
	}

	void cDeferredRenderer_vulkan::createQuadRenderCallback()
	{
		DF_ProfilingScopeCpu;

		sPipelineCreateInfo_vulkan pipeline_create_info{ .name = "deferred_quad_final" };

		pipeline_create_info.vertex_input_binding.emplace_back( 0, static_cast< uint32_t >( sizeof( cQuad_vulkan::sVertex ) ), vk::VertexInputRate::eVertex );

		pipeline_create_info.vertex_input_attribute.emplace_back( 0,
		                                                          0,
		                                                          vk::Format::eR32G32B32Sfloat,
		                                                          static_cast< uint32_t >( offsetof( cQuad_vulkan::sVertex, cQuad_vulkan::sVertex::position ) ) );
		pipeline_create_info.vertex_input_attribute.emplace_back( 1,
		                                                          0,
		                                                          vk::Format::eR32G32Sfloat,
		                                                          static_cast< uint32_t >( offsetof( cQuad_vulkan::sVertex, cQuad_vulkan::sVertex::tex_coord ) ) );

		pipeline_create_info.push_constant_ranges.emplace_back( vk::ShaderStageFlagBits::eVertex, 0, static_cast< uint32_t >( sizeof( sPushConstants ) ) );

		sDescriptorLayoutBuilder_vulkan descriptor_layout_builder{};
		descriptor_layout_builder.addBinding( 0, vk::DescriptorType::eUniformBuffer );
		descriptor_layout_builder.addBinding( 1, vk::DescriptorType::eSampledImage );
		descriptor_layout_builder.addBinding( 2, vk::DescriptorType::eSampledImage );
		descriptor_layout_builder.addBinding( 3, vk::DescriptorType::eSampledImage );
		descriptor_layout_builder.addBinding( 4, vk::DescriptorType::eSampler );
		m_deferred_layout = descriptor_layout_builder.build( vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment );

		pipeline_create_info.descriptor_layouts.push_back( m_deferred_layout.get() );

		pipeline_create_info.setShaders( helper::util::createShaderModule( "deferred_quad_final.vert" ), helper::util::createShaderModule( "deferred_quad_final.frag" ) );
		pipeline_create_info.setInputTopology( vk::PrimitiveTopology::eTriangleList );
		pipeline_create_info.setPolygonMode( vk::PolygonMode::eFill );
		pipeline_create_info.setCullMode( vk::CullModeFlagBits::eFront, vk::FrontFace::eClockwise );
		pipeline_create_info.setColorFormat( getRenderColorFormat() );
		pipeline_create_info.setDepthFormat( getRenderDepthFormat() );
		pipeline_create_info.setMultisamplingNone();
		pipeline_create_info.enableDepthTest( true, vk::CompareOp::eLessOrEqual );
		pipeline_create_info.disableBlending();

		m_deferred_screen_quad->render_callback = new cRenderCallback( "deferred_quad_final", pipeline_create_info, render_callbacks::cDefaultQuad_vulkan::deferredQuadFinal );
	}
}