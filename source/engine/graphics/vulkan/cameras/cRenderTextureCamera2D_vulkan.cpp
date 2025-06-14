#include "cRenderTextureCamera2D_vulkan.h"

#include "engine/graphics/cRenderer.h"
#include "engine/graphics/types/sSceneUniforms.h"
#include "engine/graphics/vulkan/assets/textures/cRenderTexture2D_vulkan.h"
#include "engine/graphics/vulkan/cGraphicsApi_vulkan.h"
#include "engine/graphics/vulkan/descriptor/cDescriptorWriter_vulkan.h"
#include "engine/graphics/vulkan/types/Helper_vulkan.h"
#include "engine/managers/cCameraManager.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::vulkan
{

	cRenderTextureCamera2D_vulkan::cRenderTextureCamera2D_vulkan( const sDescription& _description )
		: cRenderTextureCamera2D( _description )
	{
		DF_ProfilingScopeCpu;

		cGraphicsApi_vulkan* graphics_api = reinterpret_cast< cGraphicsApi_vulkan* >( cRenderer::getApi() );

		m_vertex_scene_uniform_buffer = helper::util::createBuffer( sizeof( sVertexSceneUniforms ),
		                                                            vk::BufferUsageFlagBits::eUniformBuffer,
		                                                            vma::MemoryUsage::eCpuToGpu,
		                                                            graphics_api->getMemoryAllocator() );

		for( sFrameData_vulkan& frame_data: graphics_api->getFrameData() )
			m_vertex_scene_descriptor_sets.push_back( frame_data.static_descriptors.allocate( sFrameData_vulkan::s_vertex_scene_descriptor_set_layout.get() ) );
	}
	cRenderTextureCamera2D_vulkan::~cRenderTextureCamera2D_vulkan()
	{
		DF_ProfilingScopeCpu;

		m_vertex_scene_descriptor_sets.clear();
		helper::util::destroyBuffer( m_vertex_scene_uniform_buffer );
	}

	void cRenderTextureCamera2D_vulkan::beginRender( const eClearFlags _clear_flags )
	{
		DF_ProfilingScopeCpu;
		cGraphicsApi_vulkan*     graphics_api = reinterpret_cast< cGraphicsApi_vulkan* >( cRenderer::getApi() );
		const sFrameData_vulkan& frame_data   = graphics_api->getCurrentFrame();
		DF_ProfilingScopeGpu( frame_data.profiling_context, frame_data.command_buffer.get() );

		cCameraManager* manager       = cCameraManager::getInstance();
		manager->m_current            = this;
		manager->m_current_is_regular = false;

		const bool color = static_cast< bool >( _clear_flags & kColor );
		const bool depth = static_cast< bool >( _clear_flags & kDepth );

		const cCommandBuffer& command_buffer = frame_data.command_buffer;
		const vk::ClearValue  clear_color_value(
            vk::ClearColorValue( m_description.clear_color.r, m_description.clear_color.g, m_description.clear_color.b, m_description.clear_color.a ) );
		constexpr vk::ClearValue clear_depth_stencil_value( vk::ClearDepthStencilValue( 1 ) );

		const vk::RenderingAttachmentInfo depth_attachment = helper::init::attachmentInfo( graphics_api->getDepthImage().image_view.get(),
		                                                                                   depth ? &clear_depth_stencil_value : nullptr,
		                                                                                   vk::ImageLayout::eDepthAttachmentOptimal );

		std::vector< vk::RenderingAttachmentInfo > color_attachments;
		color_attachments.reserve( m_textures.size() );

		for( const cUnique< cRenderTexture2D >& image: m_textures )
		{
			color_attachments.push_back( helper::init::attachmentInfo( reinterpret_cast< const cRenderTexture2D_vulkan* >( image.get() )->getImage().image_view.get(),
			                                                           color ? &clear_color_value : nullptr,
			                                                           vk::ImageLayout::eColorAttachmentOptimal ) );
		}

		command_buffer.beginRendering( graphics_api->getRenderExtent(), color_attachments, &depth_attachment );

		{
			const sVertexSceneUniforms uniforms{
				.view_projection = m_view_projection,
			};

			helper::util::setBufferData( &uniforms, sizeof( uniforms ), 0, m_vertex_scene_uniform_buffer );

			cDescriptorWriter_vulkan writer_scene;
			writer_scene.writeBuffer( 0, m_vertex_scene_uniform_buffer.buffer.get(), sizeof( uniforms ), 0, vk::DescriptorType::eUniformBuffer );
			writer_scene.updateSet( m_vertex_scene_descriptor_sets[ graphics_api->getCurrentFrameIndex() ] );
		}
	}

	void cRenderTextureCamera2D_vulkan::endRender()
	{
		DF_ProfilingScopeCpu;
		cGraphicsApi_vulkan*     graphics_api = reinterpret_cast< cGraphicsApi_vulkan* >( cRenderer::getApi() );
		const sFrameData_vulkan& frame_data   = graphics_api->getCurrentFrame();
		DF_ProfilingScopeGpu( frame_data.profiling_context, frame_data.command_buffer.get() );

		const cCommandBuffer& command_buffer = frame_data.command_buffer;
		command_buffer.endRendering();

		cCameraManager::getInstance()->m_current = nullptr;
	}
}