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
	class cTexture2D;

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

		virtual void initialize()      = 0;
		virtual void initializeImGui() = 0;
		void         initializeGui() const;

		const iFramebuffer* getDeferredFramebuffer() const { return m_deferred_framebuffer; }

		virtual void initializeDeferred() = 0;

		void setWindowMinimized( const bool _minimized ) { m_window_minimized = _minimized; }
		void setWindowResized( const bool _resized ) { m_window_resized = _resized; }

	protected:
		enum eGuiType
		{
			kRectangle,
			kBorder,
			kImage,
			kText,
		};

		struct sVertex
		{
			cVector2f position;
			cVector2f tex_coord;
			cColor    color;
			cVector2f size;
			cVector4f corner_radius;
			cVector4f border_widths;
			eGuiType  type;
		};

		void         renderGui();
		virtual void renderGui( const std::vector< sVertex >& _vertices, const cTexture2D* _texture ) {}

		iWindow* m_window;

		iFramebuffer* m_deferred_framebuffer;
		iQuad*        m_deferred_screen_quad;

		bool m_window_minimized;
		bool m_window_resized;

	private:
		static Clay_Dimensions clayTextMeasure( Clay_StringSlice _text, Clay_TextElementConfig* _config, void* _user_data );
		static void            clayErrorCallback( Clay_ErrorData _error_data );
	};
}
