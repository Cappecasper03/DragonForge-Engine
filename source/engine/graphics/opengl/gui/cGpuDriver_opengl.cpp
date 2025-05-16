#include "cGpuDriver_opengl.h"

#include <glad/glad.h>

#include "engine/graphics/api/iRenderer.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/opengl/functions/sTextureImage.h"
#include "engine/graphics/opengl/functions/sTextureParameter.h"
#include "engine/graphics/opengl/OpenGlTypes.h"
#include "engine/graphics/opengl/window/cWindow_opengl.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::opengl
{
	cGpuDriver_opengl::cGpuDriver_opengl()
	{
		m_shader_map[ ultralight::ShaderType::Fill ]     = cShader_opengl( "ultralight_fill" );
		m_shader_map[ ultralight::ShaderType::FillPath ] = cShader_opengl( "ultralight_fill_path" );
	}

	void cGpuDriver_opengl::CreateTexture( const uint32_t _texture_id, const ultralight::RefPtr< ultralight::Bitmap > _bitmap )
	{
		DF_ProfilingScopeCpu;

		sTextureEntry& texture_entry = m_texture_map[ _texture_id ];
		texture_entry.size           = cVector2i( static_cast< int >( _bitmap->height() ), static_cast< int >( _bitmap->width() ) );

		texture_entry.texture.setInteger( sTextureParameter::kMinFilter, sTextureParameter::sMinFilter::kLinear );
		texture_entry.texture.setInteger( sTextureParameter::kMagFilter, sTextureParameter::sMagFilter::kLinear );
		texture_entry.texture.setInteger( sTextureParameter::kWrapS, sTextureParameter::sWrapS::kClampToEdge );
		texture_entry.texture.setInteger( sTextureParameter::kWrapT, sTextureParameter::sWrapT::kClampToEdge );

		texture_entry.texture.bind();

		if( _bitmap->IsEmpty() )
		{
			texture_entry.texture.set2D( 0,
			                             sTextureImage::sInternalFormat::Sized::kRGBA8,
			                             cVector2i( _bitmap->width(), _bitmap->height() ),
			                             0,
			                             sTextureImage::sFormat::kBGRA,
			                             kUnsignedByte,
			                             nullptr );
		}
		else
		{
			glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
			glPixelStorei( GL_UNPACK_ROW_LENGTH, _bitmap->row_bytes() / _bitmap->bpp() );

			const void* pixels = _bitmap->LockPixels();
			switch( _bitmap->format() )
			{
				case ultralight::BitmapFormat::A8_UNORM:
				{
					texture_entry.texture.set2D( 0,
					                             sTextureImage::sInternalFormat::Sized::kR8,
					                             cVector2i( _bitmap->width(), _bitmap->height() ),
					                             0,
					                             sTextureImage::sFormat::kRed,
					                             kUnsignedByte,
					                             pixels );
					break;
				}
				case ultralight::BitmapFormat::BGRA8_UNORM_SRGB:
				{
					texture_entry.texture.set2D( 0,
					                             sTextureImage::sInternalFormat::Sized::kRGBA8,
					                             cVector2i( _bitmap->width(), _bitmap->height() ),
					                             0,
					                             sTextureImage::sFormat::kBGRA,
					                             kUnsignedByte,
					                             pixels );
					break;
				}
			}

			_bitmap->UnlockPixels();
		}

		glGenerateMipmap( GL_TEXTURE_2D );
	}

	void cGpuDriver_opengl::UpdateTexture( const uint32_t _texture_id, const ultralight::RefPtr< ultralight::Bitmap > _bitmap )
	{
		DF_ProfilingScopeCpu;

		sTextureEntry& texture_entry = m_texture_map[ _texture_id ];

		texture_entry.texture.setInteger( sTextureParameter::kMinFilter, sTextureParameter::sMinFilter::kLinear );
		texture_entry.texture.setInteger( sTextureParameter::kMagFilter, sTextureParameter::sMagFilter::kLinear );
		texture_entry.texture.setInteger( sTextureParameter::kWrapS, sTextureParameter::sWrapS::kClampToEdge );
		texture_entry.texture.setInteger( sTextureParameter::kWrapT, sTextureParameter::sWrapT::kClampToEdge );

		texture_entry.texture.bind();

		if( !_bitmap->IsEmpty() )
		{
			glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
			glPixelStorei( GL_UNPACK_ROW_LENGTH, _bitmap->row_bytes() / _bitmap->bpp() );

			const void* pixels = _bitmap->LockPixels();
			switch( _bitmap->format() )
			{
				case ultralight::BitmapFormat::A8_UNORM:
				{
					texture_entry.texture.set2D( 0,
					                             sTextureImage::sInternalFormat::Sized::kR8,
					                             cVector2i( _bitmap->width(), _bitmap->height() ),
					                             0,
					                             sTextureImage::sFormat::kRed,
					                             kUnsignedByte,
					                             pixels );
					break;
				}
				case ultralight::BitmapFormat::BGRA8_UNORM_SRGB:
				{
					texture_entry.texture.set2D( 0,
					                             sTextureImage::sInternalFormat::Sized::kRGBA8,
					                             cVector2i( _bitmap->width(), _bitmap->height() ),
					                             0,
					                             sTextureImage::sFormat::kBGRA,
					                             kUnsignedByte,
					                             pixels );
					break;
				}
			}

			_bitmap->UnlockPixels();

			glGenerateMipmap( GL_TEXTURE_2D );
		}
	}

	void cGpuDriver_opengl::DestroyTexture( const uint32_t _texture_id )
	{
		DF_ProfilingScopeCpu;

		m_texture_map.erase( _texture_id );
	}

	void cGpuDriver_opengl::CreateRenderBuffer( const uint32_t _render_buffer_id, const ultralight::RenderBuffer& _buffer )
	{
		DF_ProfilingScopeCpu;

		sRenderBufferEntry& render_buffer_entry = m_render_buffer_map[ _render_buffer_id ];
		render_buffer_entry.texture_id          = _buffer.texture_id;

		render_buffer_entry.frame_buffer.bind();
		render_buffer_entry.render_buffer.bind();
		render_buffer_entry.frame_buffer.setRenderBuffer( GL_DEPTH_STENCIL_ATTACHMENT, render_buffer_entry.render_buffer );

		sTextureEntry& texture_entry   = m_texture_map[ _buffer.texture_id ];
		texture_entry.render_buffer_id = _buffer.texture_id;

		texture_entry.texture.bind();
		render_buffer_entry.frame_buffer.setTexture2D( 0, texture_entry.texture );

		constexpr unsigned buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers( 1, buffers );
		render_buffer_entry.frame_buffer.unbind();
	}

	void cGpuDriver_opengl::DestroyRenderBuffer( const uint32_t _render_buffer_id )
	{
		DF_ProfilingScopeCpu;

		m_render_buffer_map.erase( _render_buffer_id );
	}

	void cGpuDriver_opengl::CreateGeometry( const uint32_t _geometry_id, const ultralight::VertexBuffer& _vertices, const ultralight::IndexBuffer& _indices )
	{
		DF_ProfilingScopeCpu;

		sGeometryEntry& geometry_entry = m_geometry_map[ _geometry_id ];
		geometry_entry.vertex_format   = _vertices.format;

		geometry_entry.vertex_array.bind();

		geometry_entry.vertex_buffer.bind();
		geometry_entry.vertex_buffer.setData( _vertices.size, _vertices.data, cBuffer_opengl::kDynamicDraw );

		geometry_entry.index_buffer.bind();
		geometry_entry.index_buffer.setData( _indices.size, _indices.data, cBuffer_opengl::kStaticDraw );

		switch( geometry_entry.vertex_format )
		{
			case ultralight::VertexBufferFormat::_2f_4ub_2f:
			{
				constexpr GLsizei stride = 20;

				geometry_entry.vertex_array.setAttribute( 0, 2, kFloat, stride, 0 );
				geometry_entry.vertex_array.setAttribute( 1, 4, kUnsignedByte, stride, 8, true, true );
				geometry_entry.vertex_array.setAttribute( 2, 2, kFloat, stride, 12 );

				geometry_entry.vertex_array.enableAttribute( 0 );
				geometry_entry.vertex_array.enableAttribute( 1 );
				geometry_entry.vertex_array.enableAttribute( 2 );
				break;
			}
			case ultralight::VertexBufferFormat::_2f_4ub_2f_2f_28f:
			{
				constexpr GLsizei stride = 140;

				geometry_entry.vertex_array.setAttribute( 0, 2, kFloat, stride, 0 );
				geometry_entry.vertex_array.setAttribute( 1, 4, kUnsignedByte, stride, 8, true, true );
				geometry_entry.vertex_array.setAttribute( 2, 2, kFloat, stride, 12 );
				geometry_entry.vertex_array.setAttribute( 3, 2, kFloat, stride, 20 );
				geometry_entry.vertex_array.setAttribute( 4, 4, kFloat, stride, 28 );
				geometry_entry.vertex_array.setAttribute( 5, 4, kFloat, stride, 44 );
				geometry_entry.vertex_array.setAttribute( 6, 4, kFloat, stride, 60 );
				geometry_entry.vertex_array.setAttribute( 7, 4, kFloat, stride, 76 );
				geometry_entry.vertex_array.setAttribute( 8, 4, kFloat, stride, 92 );
				geometry_entry.vertex_array.setAttribute( 9, 4, kFloat, stride, 108 );
				geometry_entry.vertex_array.setAttribute( 10, 4, kFloat, stride, 124 );
				break;
			}
		}

		geometry_entry.vertex_array.unbind();
	}

	void cGpuDriver_opengl::UpdateGeometry( const uint32_t _geometry_id, const ultralight::VertexBuffer& _vertices, const ultralight::IndexBuffer& _indices )
	{
		DF_ProfilingScopeCpu;

		const sGeometryEntry& geometry_entry = m_geometry_map.at( _geometry_id );

		geometry_entry.vertex_buffer.bind();
		geometry_entry.vertex_buffer.setData( _vertices.size, _vertices.data, cBuffer_opengl::kDynamicDraw );

		geometry_entry.index_buffer.bind();
		geometry_entry.index_buffer.setData( _indices.size, _indices.data, cBuffer_opengl::kStaticDraw );
	}

	void cGpuDriver_opengl::DestroyGeometry( const uint32_t _geometry_id )
	{
		DF_ProfilingScopeCpu;

		m_geometry_map.erase( _geometry_id );
	}
	void cGpuDriver_opengl::drawCommandList()
	{
		DF_ProfilingScopeCpu;

		glEnable( kBlend );
		glDisable( kScissorTest );
		glDisable( kDepthTest );
		glDepthFunc( GL_NEVER );
		glBlendFunc( kOne, kOneMinusSrcAlpha );

		for( const ultralight::Command& command: m_command_list )
		{
			switch( command.command_type )
			{
				case ultralight::CommandType::ClearRenderBuffer: clearRenderBuffer( command.gpu_state.render_buffer_id );
				case ultralight::CommandType::DrawGeometry:      drawGeometry( command.geometry_id, command.indices_count, command.indices_offset, command.gpu_state );
			}
		}

		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}

	void cGpuDriver_opengl::clearRenderBuffer( const uint32_t _render_buffer_id )
	{
		DF_ProfilingScopeCpu;

		if( !_render_buffer_id )
			glBindFramebuffer( GL_FRAMEBUFFER, 0 );
		else
			m_render_buffer_map.at( _render_buffer_id ).frame_buffer.bind();

		glDisable( kScissorTest );
		glClearColor( 0, 0, 0, 0 );
		glClear( GL_COLOR_BUFFER_BIT );
	}

	void cGpuDriver_opengl::drawGeometry( const uint32_t _geometry_id, const uint32_t _indices_count, const uint32_t _indices_offset, const ultralight::GPUState& _state )
	{
		DF_ProfilingScopeCpu;

		if( !_state.render_buffer_id )
			glBindFramebuffer( GL_FRAMEBUFFER, 0 );
		else
			m_render_buffer_map.at( _state.render_buffer_id ).frame_buffer.bind();

		const cWindow_opengl* window = reinterpret_cast< cWindow_opengl* >( cRenderer::getRenderInstance()->getWindow() );
		window->setViewport( cVector2i( 0 ), cVector2i( _state.viewport_width, _state.viewport_height ) );

		const cShader_opengl& shader = m_shader_map[ _state.shader_type ];
		shader.use();

		const cVector4f params = cVector4f( static_cast< float >( cRenderer::getLifeTime() / 1000 ),
		                                    static_cast< float >( _state.viewport_width ),
		                                    static_cast< float >( _state.viewport_height ),
		                                    1 );

		ultralight::Matrix transform_matrix;
		transform_matrix.Set( _state.transform );

		ultralight::Matrix projection_matrix;
		projection_matrix.SetOrthographicProjection( _state.viewport_width, _state.viewport_height, false );
		projection_matrix.Transform( transform_matrix );

		shader.setFloatVector4( "State", params );
		shader.setFloatMatrix4( "Transform", 1, &projection_matrix.GetMatrix4x4().data[ 0 ] );
		shader.setFloatVector4( "Scalar4", 2, &_state.uniform_scalar[ 0 ] );
		shader.setFloatVector4( "Vector", 8, &_state.uniform_vector[ 0 ].x );
		shader.setUnsignedInt( "ClipSize", _state.clip_size );
		shader.setFloatMatrix4( "Clip", 8, &_state.clip[ 0 ].data[ 0 ] );

		m_geometry_map[ _geometry_id ].vertex_array.bind();

		m_texture_map[ _state.texture_1_id ].texture.bind( 0 );
		m_texture_map[ _state.texture_2_id ].texture.bind( 1 );
		m_texture_map[ _state.texture_3_id ].texture.bind( 2 );

		if( _state.enable_blend )
			glEnable( kBlend );
		else
			glDisable( kBlend );

		const unsigned long long size = _indices_offset * sizeof( unsigned int );
		glDrawElements( kTriangles, static_cast< int >( _indices_count ), kUnsignedInt, &size );
		m_geometry_map[ _geometry_id ].vertex_array.unbind();
	}
}
