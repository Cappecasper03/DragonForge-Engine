#pragma once

#include <clay.h>

#include "engine/core/math/cVector.h"
#include "engine/core/utils/cColor.h"
#include "engine/core/utils/Misc.h"
#include "engine/graphics/cameras/cCamera.h"

namespace df
{
	class iWindow;
	class iFramebuffer;
	class iQuad;
	class cTexture2D;
	class cRenderTextureCamera2D;

	class iGraphicsDevice
	{
	public:
		DF_DeleteCopyAndMove( iGraphicsDevice );

		iGraphicsDevice();
		virtual ~iGraphicsDevice() = default;

		virtual void render() = 0;

		virtual void beginRendering( cCamera::eClearFlags _clear_flags, const cColor& _color = color::black ) = 0;
		virtual void endRendering() {}

		iWindow* getWindow() const { return m_window; }

		void resizeWindow( int _width = -1, int _height = -1 ) const;

		virtual void initialize()      = 0;
		virtual void initializeImGui() = 0;
		void         initializeGui() const;

		const cRenderTextureCamera2D* getDeferredCamera() const { return m_deferred_camera; }

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

		struct sVertexGui
		{
			unsigned vertex_id;
		};

		struct sPushConstantsGui
		{
			cVector4f position_tex_coord[ 6 ];
			cColor    color;
			cVector4f corner_radius;
			cVector4f border_widths;
			cVector2f size;
			float     padding1;
			float     padding2;
			eGuiType  type;
		};

		void         renderGui();
		virtual void renderGui( const sPushConstantsGui& _push_constants, const cTexture2D* _texture ) = 0;

		iWindow* m_window;

		cRenderTextureCamera2D* m_deferred_camera;
		iQuad*                  m_deferred_screen_quad;

		bool m_window_minimized;
		bool m_window_resized;

	private:
		static Clay_Dimensions clayTextMeasure( Clay_StringSlice _text, Clay_TextElementConfig* _config, void* _user_data );
		static void            clayErrorCallback( Clay_ErrorData _error_data );
	};
}
