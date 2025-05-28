#include "iTexture.h"

#include <fmt/format.h>
#include <stb_image.h>

#include "engine/core/cFileSystem.h"
#include "engine/core/Log.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	iTexture::iTexture( std::string _name )
		: m_name( std::move( _name ) )
	{}

	bool iTexture::load( const std::string& _file, const bool _mipmapped, const int _mipmaps, const bool _flip_vertically_on_load )
	{
		DF_ProfilingScopeCpu;

		stbi_set_flip_vertically_on_load( _flip_vertically_on_load );
		cVector2i      size;
		int            nr_channels;
		unsigned char* data = stbi_load( cFileSystem::getPath( _file ).data(), &size.x(), &size.y(), &nr_channels, STBI_rgb_alpha );
		m_size              = size;

		if( !data )
		{
			DF_LogWarning( fmt::format( "Failed to load texture: {}", _file ) );
			return false;
		}

		loadFromData( _file, data, size, _mipmapped, _mipmaps );

		stbi_image_free( data );
		m_file_path = _file;
		return true;
	}
}
