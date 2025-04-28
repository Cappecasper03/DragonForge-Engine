#include "cTexture_opengl.h"

#include <fmt/format.h>
#include <glad/glad.h>
#include <stb_image.h>

#include "engine/filesystem/cFileSystem.h"
#include "engine/log/Log.h"
#include "engine/profiling/ProfilingMacros.h"
#include "rendering/opengl/OpenGlTypes.h"

namespace df::opengl
{
	cTexture_opengl::cTexture_opengl( std::string _name, const int _target )
		: iTexture( std::move( _name ) )
		, m_target( _target )
	{
		DF_ProfilingScopeCpu;

		glGenTextures( 1, &m_texture );

		cTexture_opengl::bind();
		constexpr uint32_t white = 0xFFFFFFFF;
		setTexImage2D( 0, GL_RGBA, 1, 1, 0, GL_RGBA, kUnsignedByte, &white );
		cTexture_opengl::unbind();
	}

	cTexture_opengl::~cTexture_opengl()
	{
		DF_ProfilingScopeCpu;

		glDeleteTextures( 1, &m_texture );
	}

	bool cTexture_opengl::load( const std::string& _file, const bool _mipmapped, const int _mipmaps, const bool _flip_vertically_on_load )
	{
		DF_ProfilingScopeCpu;

		stbi_set_flip_vertically_on_load( _flip_vertically_on_load );
		int            width, height, nr_channels;
		unsigned char* data = stbi_load( filesystem::getPath( _file ).data(), &width, &height, &nr_channels, STBI_rgb_alpha );

		if( !data )
		{
			DF_LogWarning( fmt::format( "Failed to load texture: {}", _file ) );
			return false;
		}

		bind();
		setTexImage2D( _mipmaps, GL_RGBA, width, height, 0, GL_RGBA, kUnsignedByte, data );

		if( _mipmapped )
			glGenerateMipmap( m_target );

		unbind();
		stbi_image_free( data );
		m_file_path = _file;
		return true;
	}

	void cTexture_opengl::setTexImage2D( const int      _level,
	                                     const int      _internal_format,
	                                     const int      _width,
	                                     const int      _height,
	                                     const int      _border,
	                                     const unsigned _format,
	                                     const unsigned _type,
	                                     const void*    _pixels ) const
	{
		DF_ProfilingScopeCpu;

		glTexImage2D( m_target, _level, _internal_format, _width, _height, _border, _format, _type, _pixels );
	}

	void cTexture_opengl::setTextureParameterI( const int _name, const int _param ) const
	{
		DF_ProfilingScopeCpu;

		glTexParameteri( m_target, _name, _param );
	}

	void cTexture_opengl::setPixelStoreI( const int _name, const int _param ) const
	{
		DF_ProfilingScopeCpu;

		glPixelStorei( _name, _param );
	}

	void cTexture_opengl::bind( const int _index )
	{
		DF_ProfilingScopeCpu;

		glActiveTexture( GL_TEXTURE0 + _index );
		glBindTexture( m_target, m_texture );
	}

	void cTexture_opengl::unbind( const int _index )
	{
		DF_ProfilingScopeCpu;

		glActiveTexture( GL_TEXTURE0 + _index );
		glBindTexture( m_target, 0 );
	}
}
