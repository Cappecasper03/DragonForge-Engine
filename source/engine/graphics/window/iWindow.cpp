#include "iWindow.h"

#include <fmt/format.h>
#include <SDL3/SDL_init.h>
#include <stb_image.h>

#include "engine/core/Log.h"
#include "engine/core/cFileSystem.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{

	iWindow::iWindow()
		: m_window( nullptr )
		, m_window_size( 1200, 800 )
	{
		DF_ProfilingScopeCpu;

		SDL_Init( SDL_INIT_VIDEO );
		DF_LogMessage( "Initialized SDL" );
	}

	iWindow::~iWindow()
	{
		DF_ProfilingScopeCpu;

		if( m_window )
			SDL_DestroyWindow( m_window );

		SDL_Quit();
		DF_LogMessage( "Quit SDL" );
	}

	void iWindow::setSize( const cVector2i& _size )
	{
		DF_ProfilingScopeCpu;

		m_window_size = _size;

		if( _size.x() > 0 && _size.y() > 0 )
			SDL_SetWindowSize( m_window, _size.x(), _size.y() );
	}

	bool iWindow::create( const std::string& _window_name, const unsigned _window_flags )
	{
		DF_ProfilingScopeCpu;

		m_window = SDL_CreateWindow( _window_name.data(), m_window_size.x(), m_window_size.y(), _window_flags );
		if( !m_window )
		{
			DF_LogError( "Failed to create window" );
			return true;
		}
		DF_LogMessage( fmt::format( "Created window [{}, {}]", m_window_size.x(), m_window_size.y() ) );
		return false;
	}

	void iWindow::updateSize()
	{
		DF_ProfilingScopeCpu;

		int width = 0, height = 0;
		SDL_GetWindowSizeInPixels( m_window, &width, &height );
		m_window_size = cVector2i( width, height );
	}

	void iWindow::setCaptureMouse( const bool _capture )
	{
		DF_ProfilingScopeCpu;

		SDL_CaptureMouse( _capture );
	}

	void iWindow::setRelativeMouseMode( const bool _relative_mode ) const
	{
		DF_ProfilingScopeCpu;

		SDL_SetWindowRelativeMouseMode( m_window, _relative_mode );
	}

	void iWindow::loadIcon() const
	{
		DF_ProfilingScopeCpu;

		int         channels;
		SDL_Surface icon;
		icon.pixels = stbi_load( cFileSystem::getPath( "window.png" ).data(), &icon.w, &icon.h, &channels, 4 );
		SDL_SetWindowIcon( m_window, &icon );
	}
}