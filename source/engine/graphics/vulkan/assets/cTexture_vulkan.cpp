﻿#include "cTexture_vulkan.h"

#include <fmt/format.h>
#include <stb_image.h>

#include "engine/core/cFileSystem.h"
#include "engine/core/Log.h"
#include "engine/profiling/ProfilingMacros.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/vulkan/cRenderer_vulkan.h"
#include "engine/graphics/vulkan/types/Helper_vulkan.h"

namespace df::vulkan
{
	cTexture_vulkan::cTexture_vulkan( std::string _name )
		: iTexture( std::move( _name ) )
	{
		DF_ProfilingScopeCpu;

		constexpr uint32_t white = 0xFFFFFFFF;
		m_texture                = helper::util::createImage( &white, vk::Extent3D{ 1, 1, 1 }, vk::Format::eR8G8B8A8Unorm, vk::ImageUsageFlagBits::eSampled );
	}

	cTexture_vulkan::~cTexture_vulkan()
	{
		DF_ProfilingScopeCpu;

		if( reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() )->getLogicalDevice().waitIdle() != vk::Result::eSuccess )
			DF_LogError( "Failed to wait for device idle" );
	}

	bool cTexture_vulkan::load( const std::string& _file, const bool _mipmapped, const int _mipmaps, const bool _flip_vertically_on_load )
	{
		DF_ProfilingScopeCpu;

		stbi_set_flip_vertically_on_load( _flip_vertically_on_load );
		int            width, height, nr_channels;
		unsigned char* data = stbi_load( cFileSystem::getPath( _file ).data(), &width, &height, &nr_channels, STBI_rgb_alpha );

		if( !data )
		{
			DF_LogWarning( fmt::format( "Failed to load texture: {}", _file ) );
			return false;
		}

		const VkExtent3D size{
			.width  = static_cast< uint32_t >( width ),
			.height = static_cast< uint32_t >( height ),
			.depth  = 1,
		};

		helper::util::destroyImage( m_texture );

		m_texture = helper::util::createImage( data, size, vk::Format::eR8G8B8A8Unorm, vk::ImageUsageFlagBits::eSampled, _mipmapped, _mipmaps );

		stbi_image_free( data );
		m_path = _file;
		return true;
	}
}
