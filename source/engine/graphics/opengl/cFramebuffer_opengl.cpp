#include "cFramebuffer_opengl.h"

#include <glad/glad.h>

#include "assets/cTexture_opengl.h"
#include "engine/profiling/ProfilingMacros.h"
#include "functions/sTextureImage.h"
#include "functions/sTextureParameter.h"
#include "graphics/api/iRenderer.h"
#include "graphics/cRenderer.h"
#include "graphics/window/iWindow.h"
#include "OpenGlTypes.h"

namespace df::opengl
{
	cFramebuffer_opengl::cFramebuffer_opengl( std::string _name, const unsigned _num_render_textures, const bool _generate_render_buffer, const cVector2i& _size )
		: iFramebuffer( std::move( _name ) )
	{
		DF_ProfilingScopeCpu;

		cVector2i window_size = _size;
		if( window_size.x() < 0 || window_size.y() < 0 )
			window_size = cRenderer::getRenderInstance()->getWindow()->getSize();

		glGenFramebuffers( 1, &m_buffer );
		cFramebuffer_opengl::bind();

		if( _generate_render_buffer )
		{
			glGenRenderbuffers( 1, &m_render_buffer );
			glBindRenderbuffer( GL_RENDERBUFFER, m_render_buffer );
			glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_STENCIL, _size.x(), _size.y() );
			glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_render_buffer );
		}

		std::vector< unsigned > texture_attachments{};
		for( unsigned i = 0; i < _num_render_textures; ++i )
		{
			cTexture_opengl* texture = new cTexture_opengl( "", cTexture_opengl::k2D );

			texture->bind();
			sTextureParameter::setInteger( texture, sTextureParameter::kMinFilter, sTextureParameter::sMinFilter::kNearest );
			sTextureParameter::setInteger( texture, sTextureParameter::kMagFilter, sTextureParameter::sMagFilter::kNearest );
			sTextureImage::set2D( texture,
			                      0,
			                      sTextureImage::sInternalFormat::Base::kRGBA,
			                      window_size.x(),
			                      window_size.y(),
			                      0,
			                      sTextureImage::sFormat::kRGBA,
			                      kUnsignedByte,
			                      nullptr );
			glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture->getTexture(), 0 );

			texture_attachments.push_back( GL_COLOR_ATTACHMENT0 + i );
			render_textues.push_back( texture );
		}

		glDrawBuffers( static_cast< GLsizei >( texture_attachments.size() ), texture_attachments.data() );
		cFramebuffer_opengl::unbind();
	}

	cFramebuffer_opengl::~cFramebuffer_opengl()
	{
		DF_ProfilingScopeCpu;

		glDeleteFramebuffers( 1, &m_buffer );
	}

	void cFramebuffer_opengl::setFramebufferTexture2D( const int _attachment, const int _tex_target, const int _texture, const int _level )
	{
		DF_ProfilingScopeCpu;

		glFramebufferTexture2D( GL_RENDERBUFFER, _attachment, _tex_target, _texture, _level );
	}

	void cFramebuffer_opengl::bind()
	{
		DF_ProfilingScopeCpu;

		glBindFramebuffer( GL_FRAMEBUFFER, m_buffer );
	}

	void cFramebuffer_opengl::unbind()
	{
		DF_ProfilingScopeCpu;

		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}
}
