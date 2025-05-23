#pragma once

#include <clay.h>
#include <vector>

#include "engine/core/math/cVector.h"
#include "engine/core/utils/cColor.h"
#include "engine/core/utils/Misc.h"

namespace df
{
	class iWindow;
	class iFramebuffer;
	class iQuad;

	class iGraphicsDevice
	{
	public:
		DF_DeleteCopyAndMove( iGraphicsDevice );

		iGraphicsDevice();
		virtual ~iGraphicsDevice() = default;

		virtual void render() = 0;

		virtual void beginRendering( int _clear_buffers, const cColor& _color = color::black ) = 0;
		virtual void endRendering() {}

		iWindow* getWindow() const { return m_window; }

		void resizeWindow( int _width = -1, int _height = -1 ) const;

		virtual void initializeImGui() = 0;
		void         initializeGui() const;

		const iFramebuffer* getDeferredFramebuffer() const { return m_deferred_framebuffer; }

		virtual void initializeDeferred() = 0;

		void setWindowMinimized( const bool _minimized ) { m_window_minimized = _minimized; }
		void setWindowResized( const bool _resized ) { m_window_resized = _resized; }

	protected:
		struct sVertex
		{
			cVector2f position;
			cVector2f tex_coord;
			cColor    color;
			cVector2f size;
			cVector4f corner_radii;
			cVector4f border_widths;
			float     is_border;
		};

		void         renderGui();
		virtual void renderGuiRectangle( const std::vector< sVertex >& _vertices ) {}
		virtual void renderGuiBorder( const std::vector< sVertex >& _vertices ) {}

		iWindow* m_window;

		iFramebuffer* m_deferred_framebuffer;
		iQuad*        m_deferred_screen_quad;

		bool m_window_minimized;
		bool m_window_resized;
	};
}
