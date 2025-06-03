#include "cTexture2D_vulkan.h"

#include <fmt/format.h>

#include "engine/core/Log.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/vulkan/cGraphicsDevice_vulkan.h"
#include "engine/graphics/vulkan/types/Helper_vulkan.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::vulkan
{
	cTexture2D_vulkan::cTexture2D_vulkan( const std::string& _name )
		: cTexture2D( _name )
	{
		DF_ProfilingScopeCpu;

		constexpr uint32_t white = 0xFFFFFFFF;
		m_texture                = helper::util::createImage( &white, vk::Extent3D{ 1, 1, 1 }, vk::Format::eR8G8B8A8Unorm, vk::ImageUsageFlagBits::eSampled );
	}

	cTexture2D_vulkan::~cTexture2D_vulkan()
	{
		DF_ProfilingScopeCpu;

		if( reinterpret_cast< cGraphicsDevice_vulkan* >( cRenderer::getGraphicsDevice() )->getLogicalDevice().waitIdle() != vk::Result::eSuccess )
			DF_LogError( "Failed to wait for device idle" );
	}

	bool cTexture2D_vulkan::loadFromData( const std::string& _file_path, const void* _data, const cVector2i& _size, const bool _mipmapped, const int _mipmaps )
	{
		DF_ProfilingScopeCpu;

		const VkExtent3D extent{
			.width  = static_cast< uint32_t >( _size.width() ),
			.height = static_cast< uint32_t >( _size.height() ),
			.depth  = 1,
		};

		helper::util::destroyImage( m_texture );

		m_texture = helper::util::createImage( _data, extent, vk::Format::eR8G8B8A8Unorm, vk::ImageUsageFlagBits::eSampled, _mipmapped, _mipmaps );

		return true;
	}
}
