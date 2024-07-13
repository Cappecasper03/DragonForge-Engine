#include "cFramebuffer_vulkan.h"

#include <glm/vec2.hpp>
#include <tracy/Tracy.hpp>
#include <vulkan/vulkan.hpp>

#include "cRenderer_vulkan.h"
#include "engine/rendering/cRenderer.h"
#include "engine/rendering/iRenderer.h"
#include "misc/Helper_vulkan.h"

namespace df::vulkan
{
	cFramebuffer_vulkan::cFramebuffer_vulkan( std::string _name, const uint32_t _num_render_textures, const uint32_t _frames_in_flight, const glm::ivec2& _size )
		: iFramebuffer( std::move( _name ) )
	{
		ZoneScoped;

		const cRenderer_vulkan* renderer       = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );
		vk::Device              logical_device = renderer->getLogicalDevice();

		glm::ivec2 window_size = _size;
		if( window_size.x < 0 || window_size.y < 0 )
			window_size = renderer->getWindowSize();

		for( uint32_t i = 0; i < _frames_in_flight; ++i )
		{
			std::vector< vk::ImageView >             image_views;
			std::vector< vk::AttachmentDescription > attachment_descriptions;
			std::vector< vk::SubpassDescription >    subpass_descriptions;

			for( uint32_t j = 0; j < _num_render_textures; ++j )
			{
				m_images.push_back(
					helper::util::createImage( vk::Extent3D( window_size.x, window_size.y, 1 ), vk::Format::eR32G32B32A32Sfloat, vk::ImageUsageFlagBits::eInputAttachment ) );
				image_views.push_back( m_images.back().image_view.get() );

				attachment_descriptions.emplace_back( vk::AttachmentDescriptionFlags(),
				                                      vk::Format::eR32G32B32A32Sfloat,
				                                      vk::SampleCountFlagBits::e1,
				                                      vk::AttachmentLoadOp::eClear,
				                                      vk::AttachmentStoreOp::eStore,
				                                      vk::AttachmentLoadOp::eClear,
				                                      vk::AttachmentStoreOp::eStore,
				                                      vk::ImageLayout::eGeneral,
				                                      vk::ImageLayout::eAttachmentOptimal );

				subpass_descriptions.emplace_back( vk::SubpassDescriptionFlags(), vk::PipelineBindPoint::eGraphics );
			}

			vk::RenderPassCreateInfo render_pass_create_info( vk::RenderPassCreateFlags(), attachment_descriptions, subpass_descriptions );
			m_render_passes.push_back( logical_device.createRenderPassUnique( render_pass_create_info ).value );

			const vk::FramebufferCreateInfo framebuffer_create_info( vk::FramebufferCreateFlags(), m_render_passes.back().get(), image_views, window_size.x, window_size.y, 1 );
			m_buffers.push_back( logical_device.createFramebufferUnique( framebuffer_create_info ).value );
		}
	}

	cFramebuffer_vulkan::~cFramebuffer_vulkan()
	{
		ZoneScoped;
	}
}