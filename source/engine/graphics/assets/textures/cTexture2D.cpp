#include "cTexture2D.h"

#include <fmt/format.h>
#include <stb_image.h>

#include "engine/core/cFileSystem.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/opengl/assets/cTexture2D_opengl.h"
#include "engine/graphics/vulkan/assets/cTexture2D_vulkan.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	bool cTexture2D::uploadDataFromFile( const std::string& _file, const sTextureFormat::eFormat _format, const unsigned _mip_level, const bool _generate_mipmap, const bool _flip )
	{
		DF_ProfilingScopeCpu;

		stbi_set_flip_vertically_on_load( _flip );
		cVector2i size;
		int       nr_channels;

		unsigned char* data = nullptr;

		switch( _format )
		{
			case sTextureFormat::kRed:
			{
				data = stbi_load( cFileSystem::getPath( _file ).data(), &size.x(), &size.y(), &nr_channels, STBI_grey );
				break;
			}
			case sTextureFormat::kRGB:
			case sTextureFormat::kRGB16f:
			{
				data = stbi_load( cFileSystem::getPath( _file ).data(), &size.x(), &size.y(), &nr_channels, STBI_rgb );
				break;
			}
			case sTextureFormat::kRGBA:
			{
				data = stbi_load( cFileSystem::getPath( _file ).data(), &size.x(), &size.y(), &nr_channels, STBI_rgb_alpha );
				break;
			}
		}

		if( !data )
		{
			DF_LogWarning( fmt::format( "Failed to load texture: {}", _file ) );
			return false;
		}

		uploadData( data, _format, _mip_level, _generate_mipmap );

		stbi_image_free( data );
		return true;
	}

	cTexture2D* cTexture2D::create( const sDescription& _description )
	{
		DF_ProfilingScopeCpu;

		cTexture2D* texture = nullptr;

		switch( cRenderer::getDeviceType() )
		{
			case cRenderer::kOpenGl: texture = new opengl::cTexture2D_opengl(); break;
			case cRenderer::kVulkan: texture = new vulkan::cTexture2D_vulkan(); break;
		}

		if( !texture )
			return nullptr;

		texture->initialize( _description );

		constexpr uint32_t white = 0xFFFFFFFF;
		texture->uploadData( &white, sTextureFormat::kRed );

		return texture;
	}

	cTexture2D::sImageInfo cTexture2D::getInfoFromFile( const std::string& _file )
	{
		DF_ProfilingScopeCpu;

		cVector2i size;
		int       channels = 0;

		stbi_info( _file.data(), &size.width(), &size.height(), &channels );

		sTextureFormat::eFormat format = sTextureFormat::kRed;
		if( channels == 3 )
			format = sTextureFormat::kRGB;
		else if( channels == 4 )
			format = sTextureFormat::kRGBA;
		else
			DF_LogError( fmt::format( "Format with {} channels doesn't exist", channels ) );

		const sImageInfo info{
			.size   = size,
			.format = format,
		};
		return info;
	}
}
