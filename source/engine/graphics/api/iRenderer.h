#pragma once

#include <Ultralight/RefPtr.h>
#include <Ultralight/Renderer.h>

#include "engine/core/utils/cColor.h"
#include "engine/core/utils/Misc.h"

namespace df
{
	class iWindow;
	class iGpuDriver;
	class iFramebuffer;
	class iQuad;

	class iRenderer
	{
	public:
		DF_DeleteCopyAndMove( iRenderer );

		iRenderer()          = default;
		virtual ~iRenderer() = default;

		void         update() const;
		virtual void render() = 0;
		void         renderGUI() const;

		virtual void beginRendering( int _clear_buffers, const cColor& _color = color::black ) = 0;
		virtual void endRendering() {}

		iWindow* getWindow() const { return m_window; }

		void resizeWindow( int _width = -1, int _height = -1 ) const;

		virtual void initializeImGui() = 0;

		const iFramebuffer* getDeferredFramebuffer() const { return m_deferred_framebuffer; }

		virtual void initializeDeferred() = 0;

		void initializeGUI();

		void setWindowMinimized( const bool _minimized ) { m_window_minimized = _minimized; }
		void setWindowResized( const bool _resized ) { m_window_resized = _resized; }

	protected:
		virtual void initializeGpuDriver() = 0;

		iWindow* m_window = nullptr;

		iFramebuffer* m_deferred_framebuffer = nullptr;
		iQuad*        m_deferred_screen_quad = nullptr;

		bool m_window_minimized = false;
		bool m_window_resized   = false;

		ultralight::RefPtr< ultralight::Renderer > m_gui_renderer;
		ultralight::RefPtr< ultralight::View >     m_gui_view;
		iGpuDriver*                                m_gpu_driver;
	};
}
