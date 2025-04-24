#pragma once

#include "engine/math/cVector.h"
#include "engine/misc/cColor.h"
#include "engine/misc/Misc.h"

struct SDL_Window;

namespace df
{
	class iFramebuffer;
	class iQuad;

	class iRenderer
	{
	public:
		DF_DisableCopyAndMove( iRenderer );

		iRenderer()          = default;
		virtual ~iRenderer() = default;

		virtual void render() = 0;

		virtual void beginRendering( int _clear_buffers, const cColor& _color = color::black ) = 0;
		virtual void endRendering() {}

		SDL_Window*      getWindow() const { return m_window; }
		const cVector2i& getWindowSize() const { return m_window_size; }

		void resizeWindow( int _width = -1, int _height = -1 ) const;

		virtual void initializeImGui() = 0;

		const iFramebuffer* getDeferredFramebuffer() const { return m_deferred_framebuffer; }

		virtual void initializeDeferred() {}

		void setWindowMinimized( const bool _minimized ) { m_window_minimized = _minimized; }
		void setWindowResized( const bool _resized ) { m_window_resized = _resized; }

	protected:
		SDL_Window* m_window      = nullptr;
		cVector2i   m_window_size = { 1200, 800 };

		iFramebuffer* m_deferred_framebuffer = nullptr;
		iQuad*        m_deferred_screen_quad = nullptr;

		bool m_window_minimized = false;
		bool m_window_resized   = false;
	};
}
