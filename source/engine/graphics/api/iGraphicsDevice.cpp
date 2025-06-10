#include "iGraphicsDevice.h"

#include <fmt/format.h>

#include "engine/graphics/assets/textures/cTexture2D.h"
#include "engine/graphics/cameras/cCamera.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/gui/cText_gui.h"
#include "engine/graphics/window/iWindow.h"
#include "engine/managers/cEventManager.h"
#include "engine/managers/cFontManager.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	iGraphicsDevice::iGraphicsDevice()
		: m_window( nullptr )
		, m_deferred_framebuffer( nullptr )
		, m_deferred_screen_quad( nullptr )
		, m_window_minimized( false )
		, m_window_resized( false )
	{}

	void iGraphicsDevice::resizeWindow( const int _width, const int _height ) const
	{
		DF_ProfilingScopeCpu;

		if( _width > 0 && _height > 0 )
			m_window->setSize( cVector2i( _width, _height ) );

		Clay_SetLayoutDimensions( { static_cast< float >( m_window->getSize().width() ), static_cast< float >( m_window->getSize().height() ) } );
		cEventManager::invoke( event::on_window_resize, m_window->getSize().x(), m_window->getSize().y() );
	}

	void iGraphicsDevice::initializeGui() const
	{
		DF_ProfilingScopeCpu;

		const uint32_t   memory = Clay_MinMemorySize();
		const Clay_Arena arean  = Clay_CreateArenaWithCapacityAndMemory( memory, std::malloc( memory ) );

		Clay_Initialize( arean,
		                 Clay_Dimensions( static_cast< float >( m_window->getSize().height() ), static_cast< float >( m_window->getSize().width() ) ),
		                 { clayErrorCallback, nullptr } );

		Clay_SetMeasureTextFunction( clayTextMeasure, nullptr );
	}

	void iGraphicsDevice::renderGui()
	{
		DF_ProfilingScopeCpu;

		gui::cText_gui::clearStoredTexts();
		Clay_BeginLayout();

		cEventManager::invoke( event::render_gui );

		const Clay_RenderCommandArray command_array = Clay_EndLayout();

		static cCamera camera( "clay", cCamera::eType::kOrthographic, color::white, 90.f, -1.f, 100.f );
		camera.m_flip_y = cRenderer::getDeviceType() == cRenderer::kOpenGl;
		resizeWindow();
		camera.beginRender( cCamera::kDepth );

		for( int i = 0; i < command_array.length; ++i )
		{
			const Clay_RenderCommand& command = command_array.internalArray[ i ];

			const float& x = command.boundingBox.x;
			const float& y = command.boundingBox.y;
			const float& w = command.boundingBox.width;
			const float& h = command.boundingBox.height;

			const float min_dim_half = std::min( w, h ) * 0.5f;

			switch( command.commandType )
			{
				case CLAY_RENDER_COMMAND_TYPE_RECTANGLE:
				{
					const cVector4f radius( command.renderData.rectangle.cornerRadius.topLeft * min_dim_half,
					                        command.renderData.rectangle.cornerRadius.topRight * min_dim_half,
					                        command.renderData.rectangle.cornerRadius.bottomLeft * min_dim_half,
					                        command.renderData.rectangle.cornerRadius.bottomRight * min_dim_half );

					std::vector< sVertex > vertices( 6 );

					vertices[ 0 ].position  = cVector2f( x, y );
					vertices[ 0 ].tex_coord = cVector2f( 0, 0 );

					vertices[ 1 ].position  = cVector2f( x + w, y );
					vertices[ 1 ].tex_coord = cVector2f( 1, 0 );

					vertices[ 2 ].position  = cVector2f( x, y + h );
					vertices[ 2 ].tex_coord = cVector2f( 0, 1 );

					vertices[ 3 ].position  = cVector2f( x + w, y );
					vertices[ 3 ].tex_coord = cVector2f( 1, 0 );

					vertices[ 4 ].position  = cVector2f( x + w, y + h );
					vertices[ 4 ].tex_coord = cVector2f( 1, 1 );

					vertices[ 5 ].position  = cVector2f( x, y + h );
					vertices[ 5 ].tex_coord = cVector2f( 0, 1 );

					for( int j = 0; j < 6; ++j )
					{
						vertices[ j ].color.r = command.renderData.rectangle.backgroundColor.r;
						vertices[ j ].color.g = command.renderData.rectangle.backgroundColor.g;
						vertices[ j ].color.b = command.renderData.rectangle.backgroundColor.b;
						vertices[ j ].color.a = command.renderData.rectangle.backgroundColor.a;

						vertices[ j ].size          = cVector2f( w, h );
						vertices[ j ].corner_radius = radius;
						vertices[ j ].type          = kRectangle;
					}

					renderGui( vertices, nullptr );
					break;
				}
				case CLAY_RENDER_COMMAND_TYPE_BORDER:
				{
					const cVector4f radius( command.renderData.border.cornerRadius.topLeft * min_dim_half,
					                        command.renderData.border.cornerRadius.topRight * min_dim_half,
					                        command.renderData.border.cornerRadius.bottomLeft * min_dim_half,
					                        command.renderData.border.cornerRadius.bottomRight * min_dim_half );

					std::vector< sVertex > vertices( 6 );

					vertices[ 0 ].position  = cVector2f( x, y );
					vertices[ 0 ].tex_coord = cVector2f( 0, 0 );

					vertices[ 1 ].position  = cVector2f( x + w, y );
					vertices[ 1 ].tex_coord = cVector2f( 1, 0 );

					vertices[ 2 ].position  = cVector2f( x, y + h );
					vertices[ 2 ].tex_coord = cVector2f( 0, 1 );

					vertices[ 3 ].position  = cVector2f( x + w, y );
					vertices[ 3 ].tex_coord = cVector2f( 1, 0 );

					vertices[ 4 ].position  = cVector2f( x + w, y + h );
					vertices[ 4 ].tex_coord = cVector2f( 1, 1 );

					vertices[ 5 ].position  = cVector2f( x, y + h );
					vertices[ 5 ].tex_coord = cVector2f( 0, 1 );

					for( int j = 0; j < 6; ++j )
					{
						vertices[ j ].color.r = command.renderData.border.color.r;
						vertices[ j ].color.g = command.renderData.border.color.g;
						vertices[ j ].color.b = command.renderData.border.color.b;
						vertices[ j ].color.a = command.renderData.border.color.a;

						vertices[ j ].size          = cVector2f( w, h );
						vertices[ j ].corner_radius = radius;

						vertices[ j ].border_widths.x() = command.renderData.border.width.left;
						vertices[ j ].border_widths.y() = command.renderData.border.width.right;
						vertices[ j ].border_widths.z() = command.renderData.border.width.top;
						vertices[ j ].border_widths.w() = command.renderData.border.width.bottom;

						vertices[ j ].type = kBorder;
					}

					renderGui( vertices, nullptr );
					break;
				}
				case CLAY_RENDER_COMMAND_TYPE_TEXT:
				{
					const cFont&             font     = cFontManager::get( "roboto" );
					msdf_atlas::FontGeometry geometry = font.getGeometry();
					msdfgen::FontMetrics     metrics  = geometry.getMetrics();

					double       x1          = 0;
					const double pixel_scale = command.renderData.text.fontSize / ( metrics.ascenderY - metrics.descenderY );

					const cVector2d texel_size( 1 / static_cast< double >( font.getTexture()->getSize().width() ),
					                            1 / static_cast< double >( font.getTexture()->getSize().height() ) );

					for( int j = 0; j < command.renderData.text.stringContents.length; ++j )
					{
						const char                       character = command.renderData.text.stringContents.chars[ j ];
						const msdf_atlas::GlyphGeometry* glyph     = geometry.getGlyph( character );
						if( !glyph )
							glyph = geometry.getGlyph( '?' );
						if( !glyph )
							continue;

						cVector2d tex_coord_min;
						cVector2d tex_coord_max;
						glyph->getQuadAtlasBounds( tex_coord_min.x(), tex_coord_max.y(), tex_coord_max.x(), tex_coord_min.y() );

						tex_coord_min *= texel_size;
						tex_coord_max *= texel_size;

						cVector2d quad_min;
						cVector2d quad_max;
						glyph->getQuadPlaneBounds( quad_min.x(), quad_min.y(), quad_max.x(), quad_max.y() );

						const double y1  = quad_min.y() + quad_max.y();
						quad_min.y()    += metrics.descenderY - y1;
						quad_max.y()    += metrics.descenderY - y1;

						quad_min *= pixel_scale;
						quad_max *= pixel_scale;

						quad_min += cVector2d( x1, h );
						quad_max += cVector2d( x1, h );

						std::vector< sVertex > vertices( 6 );
						const cVector2f        start_position( x, y );

						vertices[ 0 ].position  = start_position + quad_min;
						vertices[ 0 ].tex_coord = tex_coord_min;

						vertices[ 1 ].position  = start_position + cVector2d( quad_max.x(), quad_min.y() );
						vertices[ 1 ].tex_coord = cVector2d( tex_coord_max.x(), tex_coord_min.y() );

						vertices[ 2 ].position  = start_position + cVector2d( quad_min.x(), quad_max.y() );
						vertices[ 2 ].tex_coord = cVector2d( tex_coord_min.x(), tex_coord_max.y() );

						vertices[ 3 ].position  = start_position + cVector2d( quad_max.x(), quad_min.y() );
						vertices[ 3 ].tex_coord = cVector2d( tex_coord_max.x(), tex_coord_min.y() );

						vertices[ 4 ].position  = start_position + quad_max;
						vertices[ 4 ].tex_coord = tex_coord_max;

						vertices[ 5 ].position  = start_position + cVector2d( quad_min.x(), quad_max.y() );
						vertices[ 5 ].tex_coord = cVector2d( tex_coord_min.x(), tex_coord_max.y() );

						for( int k = 0; k < 6; ++k )
						{
							vertices[ k ].color.r = command.renderData.text.textColor.r;
							vertices[ k ].color.g = command.renderData.text.textColor.g;
							vertices[ k ].color.b = command.renderData.text.textColor.b;
							vertices[ k ].color.a = command.renderData.text.textColor.a;

							vertices[ k ].size = cVector2f( w, h );
							vertices[ k ].type = kText;
						}

						renderGui( vertices, font.getTexture() );

						if( j >= command.renderData.text.stringContents.length )
							break;

						double advance = glyph->getAdvance();
						geometry.getAdvance( advance, character, command.renderData.text.stringContents.chars[ j + 1 ] );

						x1 += pixel_scale * advance + command.renderData.text.letterSpacing;
					}

					break;
				}
				case CLAY_RENDER_COMMAND_TYPE_IMAGE:
				{
					const cVector4f radius( command.renderData.image.cornerRadius.topLeft * min_dim_half,
					                        command.renderData.image.cornerRadius.topRight * min_dim_half,
					                        command.renderData.image.cornerRadius.bottomLeft * min_dim_half,
					                        command.renderData.image.cornerRadius.bottomRight * min_dim_half );

					std::vector< sVertex > vertices( 6 );

					vertices[ 0 ].position  = cVector2f( x, y );
					vertices[ 0 ].tex_coord = cVector2f( 0, 0 );

					vertices[ 1 ].position  = cVector2f( x + w, y );
					vertices[ 1 ].tex_coord = cVector2f( 1, 0 );

					vertices[ 2 ].position  = cVector2f( x, y + h );
					vertices[ 2 ].tex_coord = cVector2f( 0, 1 );

					vertices[ 3 ].position  = cVector2f( x + w, y );
					vertices[ 3 ].tex_coord = cVector2f( 1, 0 );

					vertices[ 4 ].position  = cVector2f( x + w, y + h );
					vertices[ 4 ].tex_coord = cVector2f( 1, 1 );

					vertices[ 5 ].position  = cVector2f( x, y + h );
					vertices[ 5 ].tex_coord = cVector2f( 0, 1 );

					for( int j = 0; j < 6; ++j )
					{
						vertices[ j ].color.r = command.renderData.image.backgroundColor.r;
						vertices[ j ].color.g = command.renderData.image.backgroundColor.g;
						vertices[ j ].color.b = command.renderData.image.backgroundColor.b;
						vertices[ j ].color.a = command.renderData.image.backgroundColor.a;

						vertices[ j ].size          = cVector2f( w, h );
						vertices[ j ].corner_radius = radius;
						vertices[ j ].type          = kImage;
					}

					renderGui( vertices, static_cast< const cTexture2D* >( command.renderData.image.imageData ) );
					break;
				}
			}
		}

		camera.endRender();
	}

	Clay_Dimensions iGraphicsDevice::clayTextMeasure( Clay_StringSlice _text, Clay_TextElementConfig* _config, void* /*_user_data*/ )
	{
		DF_ProfilingScopeCpu;

		const Clay_Dimensions dimensions{
			.width  = static_cast< float >( _text.length * _config->fontSize ),
			.height = static_cast< float >( _config->fontSize ),
		};

		return dimensions;
	}

	void iGraphicsDevice::clayErrorCallback( Clay_ErrorData _error_data )
	{
		DF_ProfilingScopeCpu;

		switch( _error_data.errorType )
		{
			case CLAY_ERROR_TYPE_TEXT_MEASUREMENT_FUNCTION_NOT_PROVIDED:
			{
				DF_LogError( fmt::format( "Clay, Type: Text Measurement Function Not Provided, Message: {}", _error_data.errorText.chars ) );
				break;
			}
			case CLAY_ERROR_TYPE_ARENA_CAPACITY_EXCEEDED:
			{
				DF_LogError( fmt::format( "Clay, Type: Arena Capacity Exceeded, Message: {}", _error_data.errorText.chars ) );
				break;
			}
			case CLAY_ERROR_TYPE_ELEMENTS_CAPACITY_EXCEEDED:
			{
				DF_LogError( fmt::format( "Clay, Type: Elements Capacity Exceeded, Message: {}", _error_data.errorText.chars ) );
				break;
			}
			case CLAY_ERROR_TYPE_TEXT_MEASUREMENT_CAPACITY_EXCEEDED:
			{
				DF_LogError( fmt::format( "Clay, Type: Text Measurements Capacity Exceeded, Message: {}", _error_data.errorText.chars ) );
				break;
			}
			case CLAY_ERROR_TYPE_DUPLICATE_ID:
			{
				DF_LogError( fmt::format( "Clay, Type: Duplicate Id, Message: {}", _error_data.errorText.chars ) );
				break;
			}
			case CLAY_ERROR_TYPE_FLOATING_CONTAINER_PARENT_NOT_FOUND:
			{
				DF_LogError( fmt::format( "Clay, Type: Floating Container Parent Not Found, Message: {}", _error_data.errorText.chars ) );
				break;
			}
			case CLAY_ERROR_TYPE_PERCENTAGE_OVER_1:
			{
				DF_LogError( fmt::format( "Clay, Type: Percentage over 1 ( 100% ), Message: {}", _error_data.errorText.chars ) );
				break;
			}
			case CLAY_ERROR_TYPE_INTERNAL_ERROR:
			{
				DF_LogError( fmt::format( "Clay, Type: Internal Error, Message: {}", _error_data.errorText.chars ) );
				break;
			}
		}
	}
}
