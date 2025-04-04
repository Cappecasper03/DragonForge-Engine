#include "iRenderer.h"

#include <SDL3/SDL_video.h>

#include "engine/profiling/ProfilingMacros.h"
#include "engine/managers/cEventManager.h"

namespace df
{
	void iRenderer::resizeWindow( const int _width, const int _height ) const
	{
		DF_ProfilingScopeCPU;

		if( _width > 0 && _height > 0 )
			SDL_SetWindowSize( m_window, _width, _height );

		cEventManager::invoke( event::on_window_resize, m_window_size.x, m_window_size.y );
	}
}
