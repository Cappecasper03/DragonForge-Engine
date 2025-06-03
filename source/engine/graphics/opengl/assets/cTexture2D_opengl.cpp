#include "cTexture2D_opengl.h"

#include <fmt/format.h>
#include <glad/glad.h>

#include "engine/core/cFileSystem.h"
#include "engine/graphics/opengl/OpenGlTypes.h"
#include "engine/graphics/types/sTextureFormat.h"
#include "engine/graphics/types/sTextureType.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::opengl
{
	int cTexture2D_opengl::s_type = sTextureType::toOpenGl( sTextureType::k2D );

	cTexture2D_opengl::cTexture2D_opengl( const std::string& _name )
		: cTexture2D( _name )
	{
		DF_ProfilingScopeCpu;

		glGenTextures( 1, &m_id );

		cTexture2D_opengl::bind();
		constexpr uint32_t white = 0xFFFFFFFF;
		set2D( 0, sTextureFormat::kRed, cVector2i( 1, 1 ), 0, sTextureFormat::kRed, kUnsignedByte, &white );
		cTexture2D_opengl::unbind();
	}

	cTexture2D_opengl::~cTexture2D_opengl()
	{
		DF_ProfilingScopeCpu;

		glDeleteTextures( 1, &m_id );
	}

	bool cTexture2D_opengl::loadFromData( const std::string& _file, const void* _data, const cVector2i& _size, const bool _mipmapped, const int _mipmaps )
	{
		DF_ProfilingScopeCpu;

		bind();
		set2D( _mipmaps, sTextureFormat::kRGBA, _size, 0, sTextureFormat::kRGBA, kUnsignedByte, _data );

		if( _mipmapped )
			glGenerateMipmap( s_type );

		unbind();

		return true;
	}

	void cTexture2D_opengl::set2D( const int                     _level,
	                               const sTextureFormat::eFormat _internal_format,
	                               const cVector2i&              _size,
	                               const int                     _border,
	                               const sTextureFormat::eFormat _format,
	                               const unsigned                _type,
	                               const void*                   _pixels ) const
	{
		DF_ProfilingScopeCpu;

		glTexImage2D( s_type, _level, sTextureFormat::toOpenGl( _internal_format ), _size.width(), _size.height(), _border, sTextureFormat::toOpenGl( _format ), _type, _pixels );
	}

	void cTexture2D_opengl::setInteger( const sTextureParameter::eName _name, const sTextureParameter::eParameter _param ) const
	{
		DF_ProfilingScopeCpu;

		glTexParameteri( s_type, sTextureParameter::toOpenGl( _name ), sTextureParameter::toOpenGl( _param ) );
	}

	void cTexture2D_opengl::setInteger( const sTextureParameter::eName _name, const sTextureParameter::eParameter _param[ 4 ] ) const
	{
		DF_ProfilingScopeCpu;

		const int parameters[ 4 ] = { sTextureParameter::toOpenGl( _param[ 0 ] ),
			                          sTextureParameter::toOpenGl( _param[ 1 ] ),
			                          sTextureParameter::toOpenGl( _param[ 2 ] ),
			                          sTextureParameter::toOpenGl( _param[ 3 ] ) };

		glTexParameteriv( s_type, sTextureParameter::toOpenGl( _name ), parameters );
	}

	void cTexture2D_opengl::bind( const int _index ) const
	{
		DF_ProfilingScopeCpu;

		glActiveTexture( GL_TEXTURE0 + _index );
		glBindTexture( s_type, m_id );
	}

	void cTexture2D_opengl::unbind( const int _index ) const
	{
		DF_ProfilingScopeCpu;

		glActiveTexture( GL_TEXTURE0 + _index );
		glBindTexture( s_type, 0 );
	}
}
