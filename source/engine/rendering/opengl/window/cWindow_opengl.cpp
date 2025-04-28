#include "cWindow_opengl.h"

#include <glad/glad.h>

#include "profiling/ProfilingMacros.h"

namespace df::opengl
{

	cWindow_opengl::cWindow_opengl()
		: m_context( nullptr )
	{
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 5 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	}

	cWindow_opengl::~cWindow_opengl()
	{
		DF_ProfilingScopeCpu;

		if( m_context )
			SDL_GL_DestroyContext( m_context );
	}

	bool cWindow_opengl::setSwapInterval( const eSwapInterval _swap_interval )
	{
		DF_ProfilingScopeCpu;

		return SDL_GL_SetSwapInterval( _swap_interval );
	}

	void cWindow_opengl::setViewport( const cVector2i& _offset )
	{
		DF_ProfilingScopeCpu;

		glViewport( _offset.x(), _offset.y(), m_window_size.x(), m_window_size.y() );
	}

	void cWindow_opengl::setViewport( const cVector2i& _offset, const cVector2i& _size )
	{
		DF_ProfilingScopeCpu;

		glViewport( _offset.x(), _offset.y(), _size.x(), _size.y() );
	}

	bool cWindow_opengl::create( const std::string& _window_name, const unsigned _window_flags )
	{
		DF_ProfilingScopeCpu;

		const bool result = iWindow::create( _window_name, _window_flags );
		m_context         = SDL_GL_CreateContext( m_window );
		return result;
	}

	void cWindow_opengl::swap() const
	{
		DF_ProfilingScopeCpu;

		SDL_GL_SwapWindow( m_window );
	}
}