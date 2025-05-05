#include "cTexture_opengl.h"

#include <fmt/format.h>
#include <glad/glad.h>
#include <stb_image.h>

#include "core/cFileSystem.h"
#include "core/Log.h"
#include "engine/profiling/ProfilingMacros.h"
#include "graphics/opengl/functions/sTextureImage.h"
#include "graphics/opengl/OpenGlTypes.h"

namespace df::opengl
{
	cTexture_opengl::cTexture_opengl( std::string _name, const eType _target )
		: iTexture( std::move( _name ) )
		, m_type( _target )
	{
		DF_ProfilingScopeCpu;

		glGenTextures( 1, &m_id );

		cTexture_opengl::bind();
		constexpr uint32_t white = 0xFFFFFFFF;
		sTextureImage::set2D( this, 0, sTextureImage::sInternalFormat::Base::kRGB, cVector2i( 1, 1 ), 0, sTextureImage::sFormat::kRGB, kUnsignedByte, &white );
		cTexture_opengl::unbind();
	}

	cTexture_opengl::~cTexture_opengl()
	{
		DF_ProfilingScopeCpu;

		glDeleteTextures( 1, &m_id );
	}

	bool cTexture_opengl::load( const std::string& _file, const bool _mipmapped, const int _mipmaps, const bool _flip_vertically_on_load )
	{
		DF_ProfilingScopeCpu;

		stbi_set_flip_vertically_on_load( _flip_vertically_on_load );
		cVector2i      size;
		int            nr_channels;
		unsigned char* data = stbi_load( cFileSystem::getPath( _file ).data(), &size.x(), &size.y(), &nr_channels, STBI_rgb_alpha );

		if( !data )
		{
			DF_LogWarning( fmt::format( "Failed to load texture: {}", _file ) );
			return false;
		}

		bind();
		sTextureImage::set2D( this, _mipmaps, sTextureImage::sInternalFormat::Base::kRGBA, size, 0, sTextureImage::sInternalFormat::Base::kRGBA, kUnsignedByte, data );

		if( _mipmapped )
			glGenerateMipmap( m_type );

		unbind();
		stbi_image_free( data );
		m_file_path = _file;
		return true;
	}

	void cTexture_opengl::bind( const int _index )
	{
		DF_ProfilingScopeCpu;

		glActiveTexture( GL_TEXTURE0 + _index );
		glBindTexture( m_type, m_id );
	}

	void cTexture_opengl::unbind( const int _index )
	{
		DF_ProfilingScopeCpu;

		glActiveTexture( GL_TEXTURE0 + _index );
		glBindTexture( m_type, 0 );
	}
}
