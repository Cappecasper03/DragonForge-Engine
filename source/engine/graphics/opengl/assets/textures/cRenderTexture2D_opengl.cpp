#include "cRenderTexture2D_opengl.h"

#include <glad/glad.h>

#include "engine/graphics/opengl/OpenGlTypes.h"
#include "engine/graphics/types/sTextureType.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::opengl
{
	cRenderTexture2D_opengl::cRenderTexture2D_opengl()
		: m_id( 0 )
	{}

	cRenderTexture2D_opengl::~cRenderTexture2D_opengl()
	{
		DF_ProfilingScopeCpu;

		if( m_id > 0 )
			glDeleteTextures( 1, &m_id );
	}

	void cRenderTexture2D_opengl::clear( const cColor& _color )
	{
		DF_ProfilingScopeCpu;

		glClearColor( _color.r, _color.g, _color.b, _color.a );
		glClearTexImage( m_id, 0, sTextureFormat::toOpenGlBase( sTextureFormat::kRGBA ), kFloat, &_color );
	}

	void cRenderTexture2D_opengl::bind( const int _index ) const
	{
		DF_ProfilingScopeCpu;

		glActiveTexture( GL_TEXTURE0 + _index );
		glBindTexture( sTextureType::toOpenGl( sTextureType::k2D ), m_id );
	}

	void cRenderTexture2D_opengl::unbind( const int _index ) const
	{
		DF_ProfilingScopeCpu;

		glActiveTexture( GL_TEXTURE0 + _index );
		glBindTexture( sTextureType::toOpenGl( sTextureType::k2D ), 0 );
	}

	void cRenderTexture2D_opengl::initialize( const sDescription& _description )
	{
		DF_ProfilingScopeCpu;

		cRenderTexture2D::initialize( _description );

		if( m_description.mip_levels == 0 )
			m_description.mip_levels = 1 + static_cast< unsigned >( std::floor( std::log2( std::max( m_description.size.width(), m_description.size.height() ) ) ) );

		glGenTextures( 1, &m_id );

		bind();

		glTexStorage2D( sTextureType::toOpenGl( sTextureType::k2D ),
		                static_cast< int >( m_description.mip_levels ),
		                sTextureFormat::toOpenGlInternal( m_description.format ),
		                static_cast< int >( m_description.size.width() ),
		                static_cast< int >( m_description.size.height() ) );

		unbind();
	}
}