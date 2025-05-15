#include "cGpuDriver_opengl.h"

#include <glad/glad.h>

#include "engine/graphics/opengl/functions/sTextureImage.h"
#include "engine/graphics/opengl/functions/sTextureParameter.h"
#include "engine/graphics/opengl/OpenGlTypes.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::opengl
{
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

		sRenderbufferEntry& render_buffer_entry = m_render_buffer_map[ _render_buffer_id ];
		render_buffer_entry.texture_id          = _buffer.texture_id;
		render_buffer_entry.frame_buffer.setRenderBuffer( GL_DEPTH_STENCIL_ATTACHMENT, render_buffer_entry.render_buffer );

		sTextureEntry& texture_entry   = m_texture_map[ _buffer.texture_id ];
		texture_entry.render_buffer_id = _buffer.texture_id;

		render_buffer_entry.frame_buffer.bind();
		render_buffer_entry.frame_buffer.setTexture2D( 0, texture_entry.texture );
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

	void cGpuDriver_opengl::UpdateCommandList( const ultralight::CommandList& _list )
	{
		DF_ProfilingScopeCpu;

		glEnable( kBlend );
		glDisable( kScissorTest );
		glDisable( kDepthTest );
		glDepthFunc( GL_NEVER );
		glBlendFunc( kOne, kOneMinusSrcAlpha );

		for( uint32_t i = 0; i < _list.size; ++i )
		{
			const ultralight::Command& command = _list.commands[ i ];

			switch( command.command_type )
			{
				case ultralight::CommandType::ClearRenderBuffer:
				{
					glDisable( kScissorTest );
					glClearColor( 0, 0, 0, 0 );
					glClear( GL_COLOR_BUFFER_BIT );
					break;
				}
				case ultralight::CommandType::DrawGeometry:
				{
					break;
				}
			}
		}

		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}
}
