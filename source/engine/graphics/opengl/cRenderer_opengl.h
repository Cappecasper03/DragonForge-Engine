#pragma once

#include <string>

#include "buffers/cBuffer_opengl.h"
#include "engine/core/utils/Misc.h"
#include "engine/graphics/api/iRenderer.h"

namespace df::opengl
{
	class cRenderer_opengl final : public iRenderer
	{
	public:
		DF_DeleteCopyAndMove( cRenderer_opengl );

		explicit cRenderer_opengl( const std::string& _window_name );
		~cRenderer_opengl() override;

		void render() override;

		void beginRendering( int _clear_buffers, const cColor& _color ) override;

		void initializeImGui() override;

		cBuffer_opengl m_vertex_scene_buffer;
		cBuffer_opengl m_fragment_scene_buffer;

	private:
		void initializeDeferred() override;

		void initializeGpuDriver() override;

		static void debugMessageCallback( unsigned _source, unsigned _type, unsigned _id, unsigned _severity, int _length, const char* _message, const void* _user_param );
	};
}
