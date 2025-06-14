#pragma once

#include <string>

#include "buffers/cBuffer_opengl.h"
#include "cShader_opengl.h"
#include "cVertexArray_opengl.h"
#include "engine/core/utils/Misc.h"
#include "engine/graphics/api/iGraphicsApi.h"
#include "engine/graphics/assets/textures/iSampler.h"

namespace df
{
	class iSampler;
}

namespace df::opengl
{
	class cRenderBuffer_opengl;

	class cGraphicsApi_opengl final : public iGraphicsApi
	{
	public:
		DF_DeleteCopyAndMove( cGraphicsApi_opengl );

		explicit cGraphicsApi_opengl( const std::string& _window_name );
		~cGraphicsApi_opengl() override;

		void render() override;

		void beginRendering( cCamera::eClearFlags _clear_flags, const cColor& _color ) override;

		void initialize() override;
		void initializeImGui() override;

		const iSampler* getLinearSampler() const { return m_sampler_linear.get(); }

		cBuffer_opengl m_vertex_scene_buffer;
		cBuffer_opengl m_fragment_scene_buffer;

	private:
		void renderGui( const sPushConstantsGui& _push_constants, const cTexture2D* _texture ) override;

		void initializeDeferred() override;

		static void debugMessageCallback( unsigned _source, unsigned _type, unsigned _id, unsigned _severity, int _length, const char* _message, const void* _user_param );

		cShader_opengl      m_shader_gui;
		cVertexArray_opengl m_vertex_array_gui;
		cBuffer_opengl      m_vertex_buffer_gui;
		cBuffer_opengl      m_index_buffer_gui;
		cBuffer_opengl      m_push_constant_gui;

		cUnique< iSampler > m_sampler_linear;
	};
}
