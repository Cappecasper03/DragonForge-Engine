#include "iGraphicsDevice.h"

#include "engine/graphics/window/iWindow.h"
#include "engine/managers/cEventManager.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	iGraphicsDevice::iGraphicsDevice()
		: m_window( nullptr )
		, m_deferred_framebuffer( nullptr )
		, m_deferred_screen_quad( nullptr )
		, m_window_minimized( false )
		, m_window_resized( false )
	{}

	void iGraphicsDevice::resizeWindow( const int _width, const int _height ) const
	{
		DF_ProfilingScopeCpu;

		if( _width > 0 && _height > 0 )
			m_window->setSize( cVector2i( _width, _height ) );

		cEventManager::invoke( event::on_window_resize, m_window->getSize().x(), m_window->getSize().y() );
	}
}
