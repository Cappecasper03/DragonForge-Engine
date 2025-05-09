#pragma once

#include <string>

#include "engine/misc/Misc.h"
#include "engine/graphics/api/iRenderer.h"

namespace df::opengl
{
	class cFramebuffer_opengl;

	class cRenderer_opengl final : public iRenderer
	{
	public:
		DF_DisableCopyAndMove( cRenderer_opengl );

		explicit cRenderer_opengl( const std::string& _window_name );
		~cRenderer_opengl() override;

		void render() override;

		void beginRendering( int _clear_buffers, const cColor& _color ) override;

		void initializeImGui() override;

	private:
		void initializeDeferred() override;

		static void debugMessageCallback( unsigned _source, unsigned _type, unsigned _id, unsigned _severity, int _length, const char* _message, const void* _user_param );
	};
}
