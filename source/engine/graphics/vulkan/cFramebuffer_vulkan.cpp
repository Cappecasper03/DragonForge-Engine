#include "cFramebuffer_vulkan.h"

#include "assets/cTexture2D_vulkan.h"
#include "cGraphicsDevice_vulkan.h"
#include "engine/graphics/assets/textures/cTexture2D.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/window/iWindow.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::vulkan
{
	cFramebuffer_vulkan::cFramebuffer_vulkan( const uint32_t _num_render_textures, const uint32_t _frames_in_flight, const cVector2i& _size )
	{
		DF_ProfilingScopeCpu;

		const cGraphicsDevice_vulkan* renderer = reinterpret_cast< cGraphicsDevice_vulkan* >( cRenderer::getGraphicsDevice() );

		cVector2i window_size = _size;
		if( window_size.x() < 0 || window_size.y() < 0 )
			window_size = renderer->getWindow()->getSize();

		const cTexture2D::sDescription description{
			.name       = "framebuffer_texture",
			.size       = window_size,
			.mip_levels = 1,
			.format     = sTextureFormat::kRGBA,
			.usage = sTextureUsage::kTransferSource | sTextureUsage::kTransferDestination | sTextureUsage::kStorage | sTextureUsage::kSampled | sTextureUsage::kColorAttachment,
		};

		for( uint32_t i = 0; i < _frames_in_flight; ++i )
		{
			std::vector< cTexture2D_vulkan* > images;
			images.reserve( _num_render_textures );

			for( uint32_t j = 0; j < _num_render_textures; ++j )
				images.push_back( reinterpret_cast< cTexture2D_vulkan* >( cTexture2D::create( description ) ) );

			m_frame_images.push_back( std::move( images ) );
		}
	}

	cFramebuffer_vulkan::~cFramebuffer_vulkan()
	{
		DF_ProfilingScopeCpu;

		for( const std::vector< cTexture2D_vulkan* >& frame: m_frame_images )
		{
			for( const cTexture2D_vulkan* image: frame )
				delete image;
		}
	}
}