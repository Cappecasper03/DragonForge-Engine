#pragma once

#include <string>

#include "buffers/cBuffer_opengl.h"
#include "engine/core/utils/Misc.h"
#include "engine/graphics/api/iGraphicsDevice.h"

namespace df::opengl
{
	class cGraphicsDevice_opengl final : public iGraphicsDevice
	{
	public:
		DF_DeleteCopyAndMove( cGraphicsDevice_opengl );

		explicit cGraphicsDevice_opengl( const std::string& _window_name );
		~cGraphicsDevice_opengl() override;

		void render() override;

		void beginRendering( int _clear_buffers, const cColor& _color ) override;

		void initializeImGui() override;

		cBuffer_opengl m_vertex_scene_buffer;
		cBuffer_opengl m_fragment_scene_buffer;

	private:
		void initializeDeferred() override;

		static void debugMessageCallback( unsigned _source, unsigned _type, unsigned _id, unsigned _severity, int _length, const char* _message, const void* _user_param );
	};
}
