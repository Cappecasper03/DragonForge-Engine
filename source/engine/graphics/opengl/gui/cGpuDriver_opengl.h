#pragma once

#include <map>
#include <Ultralight/platform/GPUDriver.h>

#include "engine/core/utils/Misc.h"
#include "engine/graphics/opengl/assets/cTexture_opengl.h"
#include "engine/graphics/opengl/buffers/cBuffer_opengl.h"
#include "engine/graphics/opengl/buffers/cFrameBuffer_opengl.h"
#include "engine/graphics/opengl/cVertexArray_opengl.h"

namespace df::opengl
{
	class cGpuDriver_opengl final : public ultralight::GPUDriver
	{
	public:
		DF_DeleteCopyAndMove( cGpuDriver_opengl );

		cGpuDriver_opengl()           = default;
		~cGpuDriver_opengl() override = default;

		void BeginSynchronize() override {}
		void EndSynchronize() override {}

		uint32_t NextTextureId() override { return static_cast< uint32_t >( m_texture_map.size() ); }

		void CreateTexture( uint32_t _texture_id, ultralight::RefPtr< ultralight::Bitmap > _bitmap ) override;
		void UpdateTexture( uint32_t _texture_id, ultralight::RefPtr< ultralight::Bitmap > _bitmap ) override;
		void DestroyTexture( uint32_t _texture_id ) override;

		uint32_t NextRenderBufferId() override { return static_cast< uint32_t >( m_render_buffer_map.size() ); }
		void     CreateRenderBuffer( uint32_t _render_buffer_id, const ultralight::RenderBuffer& _buffer ) override;
		void     DestroyRenderBuffer( uint32_t _render_buffer_id ) override;

		uint32_t NextGeometryId() override { return static_cast< uint32_t >( m_geometry_map.size() ); }
		void     CreateGeometry( uint32_t _geometry_id, const ultralight::VertexBuffer& _vertices, const ultralight::IndexBuffer& _indices ) override;
		void     UpdateGeometry( uint32_t _geometry_id, const ultralight::VertexBuffer& _vertices, const ultralight::IndexBuffer& _indices ) override;
		void     DestroyGeometry( uint32_t _geometry_id ) override;

		void UpdateCommandList( const ultralight::CommandList& _list ) override;

	private:
		struct sTextureEntry
		{
			uint32_t        render_buffer_id;
			cVector2i       size;
			cTexture_opengl texture = cTexture_opengl( "", cTexture_opengl::k2D );
		};

		struct sRenderbufferEntry
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

		std::map< uint32_t, sTextureEntry >      m_texture_map;
		std::map< uint32_t, sRenderbufferEntry > m_render_buffer_map;
		std::map< uint32_t, sGeometryEntry >     m_geometry_map;
	};
}
