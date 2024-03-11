#pragma once

#include "../iRenderer.h"
#include "engine/misc/Misc.h"

namespace df::opengl
{
	class cRenderer;
	class cFramebuffer;

	class cRenderer final : public iRenderer
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( cRenderer )

		friend cRenderer;

		cRenderer();
		~cRenderer() override;

		void render() override;

		void clearBuffers( int _buffers, const cColor& _color ) override;

		void resizeWindow( int _width = -1, int _height = -1 ) override;
		void setCursorInputMode( int _value ) override;

	private:
		void initializeDeferred();

		static void framebufferSizeCallback( GLFWwindow* _window, int _width, int _height );
		static void debugMessageCallback( unsigned _source, unsigned _type, unsigned _id, unsigned _severity, int _length, const char* _message, const void* _user_param );
	};
}
