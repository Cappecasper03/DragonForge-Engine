#pragma once

#include <map>

#include "engine/core/utils/Misc.h"
#include "engine/graphics/api/iGpuDriver.h"
#include "engine/graphics/opengl/assets/cTexture_opengl.h"
#include "engine/graphics/opengl/buffers/cBuffer_opengl.h"
#include "engine/graphics/opengl/buffers/cFrameBuffer_opengl.h"
#include "engine/graphics/opengl/cShader_opengl.h"
#include "engine/graphics/opengl/cVertexArray_opengl.h"

namespace df::opengl
{
	class cGpuDriver_opengl final : public iGpuDriver
	{
	public:
		DF_DeleteCopyAndMove( cGpuDriver_opengl );

		cGpuDriver_opengl();
		~cGpuDriver_opengl() override = default;

		void CreateTexture( uint32_t _texture_id, ultralight::RefPtr< ultralight::Bitmap > _bitmap ) override;
		void UpdateTexture( uint32_t _texture_id, ultralight::RefPtr< ultralight::Bitmap > _bitmap ) override;
		void DestroyTexture( uint32_t _texture_id ) override;

		void CreateRenderBuffer( uint32_t _render_buffer_id, const ultralight::RenderBuffer& _buffer ) override;
		void DestroyRenderBuffer( uint32_t _render_buffer_id ) override;

		void CreateGeometry( uint32_t _geometry_id, const ultralight::VertexBuffer& _vertices, const ultralight::IndexBuffer& _indices ) override;
		void UpdateGeometry( uint32_t _geometry_id, const ultralight::VertexBuffer& _vertices, const ultralight::IndexBuffer& _indices ) override;
		void DestroyGeometry( uint32_t _geometry_id ) override;

		void drawCommandList() override;

	private:
		struct sTextureEntry
		{
			uint32_t        render_buffer_id;
			cVector2i       size;
			cTexture_opengl texture = cTexture_opengl( "", cTexture_opengl::k2D );
		};

		struct sRenderBufferEntry
		{
			uint32_t             texture_id;
			cFrameBuffer_opengl  frame_buffer;
			cRenderBuffer_opengl render_buffer;
		};

		struct sGeometryEntry
		{
			ultralight::VertexBufferFormat vertex_format;
			cVertexArray_opengl            vertex_array;
			cBuffer_opengl                 vertex_buffer = cBuffer_opengl( cBuffer_opengl::kVertex );
			cBuffer_opengl                 index_buffer  = cBuffer_opengl( cBuffer_opengl::kIndex );
		};

		void clearRenderBuffer( uint32_t _render_buffer_id ) override;

		void drawGeometry( uint32_t _geometry_id, uint32_t _indices_count, uint32_t _indices_offset, const ultralight::GPUState& _state ) override;

		std::map< uint32_t, sTextureEntry >                m_texture_map;
		std::map< uint32_t, sRenderBufferEntry >           m_render_buffer_map;
		std::map< uint32_t, sGeometryEntry >               m_geometry_map;
		std::map< ultralight::ShaderType, cShader_opengl > m_shader_map;
	};
}
