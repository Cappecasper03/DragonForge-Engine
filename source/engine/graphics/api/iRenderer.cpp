#include "iRenderer.h"

#include <AppCore/Platform.h>
#include <Ultralight/platform/Platform.h>

#include "engine/core/cFileSystem.h"
#include "engine/graphics/window/iWindow.h"
#include "engine/managers/cEventManager.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	void iRenderer::update() const
	{
		DF_ProfilingScopeCpu;

		m_gui_renderer->Update();
	}

	void iRenderer::renderGUI() const
	{
		DF_ProfilingScopeCpu;

		m_gui_renderer->RefreshDisplay( 0 );
		m_gui_renderer->Render();
	}

	void iRenderer::resizeWindow( const int _width, const int _height ) const
	{
		DF_ProfilingScopeCpu;

		if( _width > 0 && _height > 0 )
			m_window->setSize( cVector2i( _width, _height ) );

		cEventManager::invoke( event::on_window_resize, m_window->getSize().x(), m_window->getSize().y() );
	}

	void iRenderer::initializeGUI()
	{
		DF_ProfilingScopeCpu;

		initializeGpuDriver();

		ultralight::Platform::instance().set_gpu_driver( m_gpu_driver );
		ultralight::Platform::instance().set_file_system( ultralight::GetPlatformFileSystem( cFileSystem::getGameDirectory().data() ) );
		ultralight::Platform::instance().set_font_loader( ultralight::GetPlatformFontLoader() );
		ultralight::Platform::instance().set_logger( ultralight::GetDefaultLogger( "ultralight.log" ) );

		m_gui_renderer = ultralight::Renderer::Create();

		const ultralight::ViewConfig config{
			.is_accelerated = true,
		};

		m_gui_view = m_gui_renderer->CreateView( m_window->getSize().x(), m_window->getSize().y(), config, nullptr );
		m_gui_view->LoadHTML( "<h1>Hello World!</h1>" );
	}
}
