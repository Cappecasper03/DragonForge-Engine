#include "cRenderTextureCamera2D_vulkan.h"

#include "engine/graphics/cRenderer.h"
#include "engine/graphics/types/sSceneUniforms.h"
#include "engine/graphics/vulkan/assets/textures/cRenderTexture2D_vulkan.h"
#include "engine/graphics/vulkan/cGraphicsDevice_vulkan.h"
#include "engine/graphics/vulkan/descriptor/cDescriptorWriter_vulkan.h"
#include "engine/graphics/vulkan/types/Helper_vulkan.h"
#include "engine/managers/cCameraManager.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::vulkan
{

	cRenderTextureCamera2D_vulkan::cRenderTextureCamera2D_vulkan( const sDescription& _description )
		: cRenderTextureCamera2D( _description )
	{}

	void cRenderTextureCamera2D_vulkan::beginRender( const int _clear_buffers )
	{
		DF_ProfilingScopeCpu;
		cGraphicsDevice_vulkan*  graphics_device = reinterpret_cast< cGraphicsDevice_vulkan* >( cRenderer::getGraphicsDevice() );
		const sFrameData_vulkan& frame_data      = graphics_device->getCurrentFrame();
		DF_ProfilingScopeGpu( frame_data.profiling_context, frame_data.command_buffer.get() );

		cCameraManager* manager = cCameraManager::getInstance();
		m_previous              = manager->m_current;
		manager->m_current      = this;

		const bool color = _clear_buffers & kColor;
		const bool depth = _clear_buffers & kDepth;

		const cCommandBuffer& command_buffer = frame_data.command_buffer;
		const vk::ClearValue  clear_color_value(
            vk::ClearColorValue( m_description.clear_color.r, m_description.clear_color.g, m_description.clear_color.b, m_description.clear_color.a ) );
		constexpr vk::ClearValue clear_depth_stencil_value( vk::ClearDepthStencilValue( 1 ) );

		const vk::RenderingAttachmentInfo depth_attachment = helper::init::attachmentInfo( graphics_device->getDepthImage().image_view.get(),
		                                                                                   depth ? &clear_depth_stencil_value : nullptr,
		                                                                                   vk::ImageLayout::eDepthAttachmentOptimal );

		std::vector< vk::RenderingAttachmentInfo > color_attachments;
		color_attachments.reserve( m_textures.size() );

		for( const cRenderTexture2D* image: m_textures )
		{
			color_attachments.push_back( helper::init::attachmentInfo( reinterpret_cast< const cRenderTexture2D_vulkan* >( image )->getImage().image_view.get(),
			                                                           color ? &clear_color_value : nullptr,
			                                                           vk::ImageLayout::eColorAttachmentOptimal ) );
		}

		command_buffer.beginRendering( graphics_device->getRenderExtent(), color_attachments, &depth_attachment );

		{
			const sAllocatedBuffer_vulkan& buffer = frame_data.getVertexSceneBuffer();
			const vk::DescriptorSet&       set    = frame_data.getVertexDescriptorSet();

			const sVertexSceneUniforms uniforms{
				.view_projection = m_view_projection,
			};

			helper::util::setBufferData( &uniforms, sizeof( uniforms ), 0, buffer );

			cDescriptorWriter_vulkan writer_scene;
			writer_scene.writeBuffer( 0, buffer.buffer.get(), sizeof( uniforms ), 0, vk::DescriptorType::eUniformBuffer );
			writer_scene.updateSet( set );
		}
	}

	void cRenderTextureCamera2D_vulkan::endRender()
	{
		DF_ProfilingScopeCpu;
		cGraphicsDevice_vulkan*  graphics_device = reinterpret_cast< cGraphicsDevice_vulkan* >( cRenderer::getGraphicsDevice() );
		const sFrameData_vulkan& frame_data      = graphics_device->getCurrentFrame();
		DF_ProfilingScopeGpu( frame_data.profiling_context, frame_data.command_buffer.get() );

		const cCommandBuffer& command_buffer = frame_data.command_buffer;
		command_buffer.endRendering();

		cCameraManager::getInstance()->m_current = m_previous;
		m_previous                               = nullptr;
	}
}