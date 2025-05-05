#include "iRenderer.h"

#include "engine/managers/cEventManager.h"
#include "engine/profiling/ProfilingMacros.h"
#include "graphics/window/iWindow.h"

namespace df
{
	void iRenderer::resizeWindow( const int _width, const int _height ) const
	{
		DF_ProfilingScopeCpu;

		if( _width > 0 && _height > 0 )
			m_window->setSize( cVector2i( _width, _height ) );

		cEventManager::invoke( event::on_window_resize, m_window->getSize().x(), m_window->getSize().y() );
	}
}
