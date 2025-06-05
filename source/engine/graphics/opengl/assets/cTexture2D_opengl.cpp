#include "cTexture2D_opengl.h"

#include <glad/glad.h>

#include "engine/core/cFileSystem.h"
#include "engine/graphics/opengl/OpenGlTypes.h"
#include "engine/graphics/types/sTextureFormat.h"
#include "engine/graphics/types/sTextureType.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::opengl
{
	cTexture2D_opengl::cTexture2D_opengl()
		: m_id( 0 )
	{}

	cTexture2D_opengl::~cTexture2D_opengl()
	{
		DF_ProfilingScopeCpu;

		if( m_id > 0 )
			glDeleteTextures( 1, &m_id );
	}

	void cTexture2D_opengl::clear( const cColor& _color )
	{
		DF_ProfilingScopeCpu;

		glClearColor( _color.r, _color.g, _color.b, _color.a );
		glClearTexImage( m_id, 0, sTextureFormat::toOpenGlBase( sTextureFormat::kRGBA ), kFloat, &_color );
	}

	void cTexture2D_opengl::uploadData( const void* _data, const sTextureFormat::eFormat _format, const unsigned _mip_level, const bool _generate_mipmap )
	{
		DF_ProfilingScopeCpu;

		bind();

		glTexSubImage2D( sTextureType::toOpenGl( sTextureType::k2D ),
		                 static_cast< int >( _mip_level ),
		                 0,
		                 0,
		                 static_cast< int >( m_description.size.width() ),
		                 static_cast< int >( m_description.size.height() ),
		                 sTextureFormat::toOpenGlBase( _format ),
		                 GL_UNSIGNED_BYTE,
		                 _data );

		if( _generate_mipmap )
			glGenerateMipmap( sTextureType::toOpenGl( sTextureType::k2D ) );

		unbind();
	}

	void cTexture2D_opengl::setInteger( const sTextureParameter::eName _name, const sTextureParameter::eParameter _param ) const
	{
		DF_ProfilingScopeCpu;

		glTexParameteri( sTextureType::toOpenGl( sTextureType::k2D ), sTextureParameter::toOpenGl( _name ), sTextureParameter::toOpenGl( _param ) );
	}

	void cTexture2D_opengl::setInteger( const sTextureParameter::eName _name, const sTextureParameter::eParameter _param[ 4 ] ) const
	{
		DF_ProfilingScopeCpu;

		const int parameters[ 4 ] = { sTextureParameter::toOpenGl( _param[ 0 ] ),
			                          sTextureParameter::toOpenGl( _param[ 1 ] ),
			                          sTextureParameter::toOpenGl( _param[ 2 ] ),
			                          sTextureParameter::toOpenGl( _param[ 3 ] ) };

		glTexParameteriv( sTextureType::toOpenGl( sTextureType::k2D ), sTextureParameter::toOpenGl( _name ), parameters );
	}

	void cTexture2D_opengl::bind( const int _index ) const
	{
		DF_ProfilingScopeCpu;

		glActiveTexture( GL_TEXTURE0 + _index );
		glBindTexture( sTextureType::toOpenGl( sTextureType::k2D ), m_id );
	}

	void cTexture2D_opengl::unbind( const int _index ) const
	{
		DF_ProfilingScopeCpu;

		glActiveTexture( GL_TEXTURE0 + _index );
		glBindTexture( sTextureType::toOpenGl( sTextureType::k2D ), 0 );
	}

	void cTexture2D_opengl::initialize( const sDescription& _description )
	{
		DF_ProfilingScopeCpu;

		cTexture2D::initialize( _description );

		if( m_description.mip_levels == 0 )
			m_description.mip_levels = 1 + static_cast< const unsigned >( std::floor( std::log2( std::max( m_description.size.width(), m_description.size.height() ) ) ) );

		glGenTextures( 1, &m_id );

		bind();

		glTexStorage2D( sTextureType::toOpenGl( sTextureType::k2D ),
		                static_cast< int >( m_description.mip_levels ),
		                sTextureFormat::toOpenGlInternal( m_description.format ),
		                static_cast< int >( m_description.size.width() ),
		                static_cast< int >( m_description.size.height() ) );

		setInteger( sTextureParameter::kMinFilter, m_description.mip_levels > 1 ? sTextureParameter::kLinearMipmapLinear : sTextureParameter::kLinear );
		setInteger( sTextureParameter::kMagFilter, sTextureParameter::kLinear );
		setInteger( sTextureParameter::kWrapS, sTextureParameter::kRepeat );
		setInteger( sTextureParameter::kWrapT, sTextureParameter::kRepeat );

		unbind();
	}
}
