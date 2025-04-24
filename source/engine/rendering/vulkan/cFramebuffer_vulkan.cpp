#include "cFramebuffer_vulkan.h"

#include <glm/vec2.hpp>
#include <vulkan/vulkan.hpp>

#include "cRenderer_vulkan.h"
#include "engine/profiling/ProfilingMacros.h"
#include "engine/rendering/cRenderer.h"
#include "engine/rendering/iRenderer.h"
#include "types/Helper_vulkan.h"

namespace df::vulkan
{
	cFramebuffer_vulkan::cFramebuffer_vulkan( std::string _name, const uint32_t _num_render_textures, const uint32_t _frames_in_flight, const cVector2i& _size )
		: iFramebuffer( std::move( _name ) )
	{
		DF_ProfilingScopeCpu;

		const cRenderer_vulkan* renderer = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );

		cVector2i window_size = _size;
		if( window_size.x() < 0 || window_size.y() < 0 )
			window_size = renderer->getWindowSize();

		for( uint32_t i = 0; i < _frames_in_flight; ++i )
		{
			std::vector< sAllocatedImage_vulkan > images;
			images.reserve( _num_render_textures );

			for( uint32_t j = 0; j < _num_render_textures; ++j )
			{
				images.push_back( helper::util::createImage( vk::Extent3D( window_size.x(), window_size.y(), 1 ),
				                                             vk::Format::eR8G8B8A8Unorm,
				                                             vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eStorage
				                                                 | vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled ) );
			}

			m_frame_images.push_back( std::move( images ) );
		}
	}
}