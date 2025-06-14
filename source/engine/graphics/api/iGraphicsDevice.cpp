﻿#include "iGraphicsDevice.h"

#include <fmt/format.h>

#include "engine/graphics/assets/textures/cTexture2D.h"
#include "engine/graphics/cameras/cCamera.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/gui/cText_gui.h"
#include "engine/graphics/window/iWindow.h"
#include "engine/managers/cCameraManager.h"
#include "engine/managers/cEventManager.h"
#include "engine/managers/cFontManager.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	iGraphicsDevice::iGraphicsDevice()
		: m_window( nullptr )
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

		if( !command_array.length )
			return;

		cCameraManager::getInstance()->m_camera_gui->beginRender( cCamera::kDepth );

		for( int i = 0; i < command_array.length; ++i )
		{
			const Clay_RenderCommand& command = command_array.internalArray[ i ];

			const float& x = command.boundingBox.x;
			const float& y = command.boundingBox.y;
			const float& w = command.boundingBox.width;
			const float& h = command.boundingBox.height;

			const float min_dim_half = std::min( w, h ) * 0.5f;

			sPushConstantsGui push_constants{};

			push_constants.position_tex_coord[ 0 ] = cVector4f( x, y, 0, 0 );
			push_constants.position_tex_coord[ 1 ] = cVector4f( x + w, y, 1, 0 );
			push_constants.position_tex_coord[ 2 ] = cVector4f( x, y + h, 0, 1 );
			push_constants.position_tex_coord[ 3 ] = cVector4f( x + w, y, 1, 0 );
			push_constants.position_tex_coord[ 4 ] = cVector4f( x + w, y + h, 1, 1 );
			push_constants.position_tex_coord[ 5 ] = cVector4f( x, y + h, 0, 1 );

			push_constants.size = cVector2f( w, h );

			switch( command.commandType )
			{
				case CLAY_RENDER_COMMAND_TYPE_RECTANGLE:
				{
					const cVector4f radius( command.renderData.rectangle.cornerRadius.topLeft * min_dim_half,
					                        command.renderData.rectangle.cornerRadius.topRight * min_dim_half,
					                        command.renderData.rectangle.cornerRadius.bottomLeft * min_dim_half,
					                        command.renderData.rectangle.cornerRadius.bottomRight * min_dim_half );

					push_constants.color.r = command.renderData.rectangle.backgroundColor.r;
					push_constants.color.g = command.renderData.rectangle.backgroundColor.g;
					push_constants.color.b = command.renderData.rectangle.backgroundColor.b;
					push_constants.color.a = command.renderData.rectangle.backgroundColor.a;

					push_constants.corner_radius = radius;
					push_constants.type          = kRectangle;

					renderGui( push_constants, nullptr );
					break;
				}
				case CLAY_RENDER_COMMAND_TYPE_BORDER:
				{
					const cVector4f radius( command.renderData.border.cornerRadius.topLeft * min_dim_half,
					                        command.renderData.border.cornerRadius.topRight * min_dim_half,
					                        command.renderData.border.cornerRadius.bottomLeft * min_dim_half,
					                        command.renderData.border.cornerRadius.bottomRight * min_dim_half );

					push_constants.color.r = command.renderData.border.color.r;
					push_constants.color.g = command.renderData.border.color.g;
					push_constants.color.b = command.renderData.border.color.b;
					push_constants.color.a = command.renderData.border.color.a;

					push_constants.corner_radius = radius;

					push_constants.border_widths.x() = command.renderData.border.width.left;
					push_constants.border_widths.y() = command.renderData.border.width.right;
					push_constants.border_widths.z() = command.renderData.border.width.top;
					push_constants.border_widths.w() = command.renderData.border.width.bottom;

					push_constants.type = kBorder;

					renderGui( push_constants, nullptr );
					break;
				}
				case CLAY_RENDER_COMMAND_TYPE_TEXT:
				{
					const cFont&             font     = cFontManager::get( command.renderData.text.fontId );
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

						const cVector2f start_position( x, y );

						push_constants.position_tex_coord[ 0 ].x() = ( start_position + quad_min ).x();
						push_constants.position_tex_coord[ 0 ].y() = ( start_position + quad_min ).y();
						push_constants.position_tex_coord[ 0 ].z() = tex_coord_min.x();
						push_constants.position_tex_coord[ 0 ].w() = tex_coord_min.y();

						push_constants.position_tex_coord[ 1 ].x() = ( start_position + cVector2d( quad_max.x(), quad_min.y() ) ).x();
						push_constants.position_tex_coord[ 1 ].y() = ( start_position + cVector2d( quad_max.x(), quad_min.y() ) ).y();
						push_constants.position_tex_coord[ 1 ].z() = cVector2d( tex_coord_max.x(), tex_coord_min.y() ).x();
						push_constants.position_tex_coord[ 1 ].w() = cVector2d( tex_coord_max.x(), tex_coord_min.y() ).y();

						push_constants.position_tex_coord[ 2 ].x() = ( start_position + cVector2d( quad_min.x(), quad_max.y() ) ).x();
						push_constants.position_tex_coord[ 2 ].y() = ( start_position + cVector2d( quad_min.x(), quad_max.y() ) ).y();
						push_constants.position_tex_coord[ 2 ].z() = cVector2d( tex_coord_min.x(), tex_coord_max.y() ).x();
						push_constants.position_tex_coord[ 2 ].w() = cVector2d( tex_coord_min.x(), tex_coord_max.y() ).y();

						push_constants.position_tex_coord[ 3 ].x() = ( start_position + cVector2d( quad_max.x(), quad_min.y() ) ).x();
						push_constants.position_tex_coord[ 3 ].y() = ( start_position + cVector2d( quad_max.x(), quad_min.y() ) ).y();
						push_constants.position_tex_coord[ 3 ].z() = cVector2d( tex_coord_max.x(), tex_coord_min.y() ).x();
						push_constants.position_tex_coord[ 3 ].w() = cVector2d( tex_coord_max.x(), tex_coord_min.y() ).y();

						push_constants.position_tex_coord[ 4 ].x() = ( start_position + quad_max ).x();
						push_constants.position_tex_coord[ 4 ].y() = ( start_position + quad_max ).y();
						push_constants.position_tex_coord[ 4 ].z() = tex_coord_max.x();
						push_constants.position_tex_coord[ 4 ].w() = tex_coord_max.y();

						push_constants.position_tex_coord[ 5 ].x() = ( start_position + cVector2d( quad_min.x(), quad_max.y() ) ).x();
						push_constants.position_tex_coord[ 5 ].y() = ( start_position + cVector2d( quad_min.x(), quad_max.y() ) ).y();
						push_constants.position_tex_coord[ 5 ].z() = cVector2d( tex_coord_min.x(), tex_coord_max.y() ).x();
						push_constants.position_tex_coord[ 5 ].w() = cVector2d( tex_coord_min.x(), tex_coord_max.y() ).y();

						push_constants.color.r = command.renderData.text.textColor.r;
						push_constants.color.g = command.renderData.text.textColor.g;
						push_constants.color.b = command.renderData.text.textColor.b;
						push_constants.color.a = command.renderData.text.textColor.a;

						push_constants.type = kText;

						renderGui( push_constants, font.getTexture() );

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

					push_constants.color.r = command.renderData.image.backgroundColor.r;
					push_constants.color.g = command.renderData.image.backgroundColor.g;
					push_constants.color.b = command.renderData.image.backgroundColor.b;
					push_constants.color.a = command.renderData.image.backgroundColor.a;

					push_constants.corner_radius = radius;
					push_constants.type          = kImage;

					renderGui( push_constants, static_cast< const cTexture2D* >( command.renderData.image.imageData ) );
					break;
				}
				default: DF_LogWarning( fmt::format( "Clay render command type not implemented: {}", static_cast< std::uint8_t >( command.commandType ) ) ); break;
			}
		}

		cCameraManager::getInstance()->m_camera_gui->endRender();
	}

	Clay_Dimensions iGraphicsDevice::clayTextMeasure( Clay_StringSlice _text, Clay_TextElementConfig* _config, void* /*_user_data*/ )
	{
		DF_ProfilingScopeCpu;

		if( !_text.chars || _text.length == 0 )
		{
			return Clay_Dimensions{
				.width  = 0,
				.height = 0,
			};
		}

		const cFont&                    font     = cFontManager::get( _config->fontId );
		const msdf_atlas::FontGeometry& geometry = font.getGeometry();
		const msdfgen::FontMetrics&     metrics  = geometry.getMetrics();

		const double scale = _config->fontSize / ( metrics.ascenderY - metrics.descenderY );

		float width = 0;
		for( int i = 0; i < _text.length; ++i )
		{
			const char current = _text.chars[ i ];

			double advance = geometry.getGlyph( current )->getAdvance();

			if( i < _text.length - 1 )
			{
				const char next = _text.chars[ i + 1 ];
				geometry.getAdvance( advance, current, next );
			}

			width += static_cast< float >( advance * scale );
		}

		return Clay_Dimensions{
			.width  = width,
			.height = static_cast< float >( _config->fontSize ),
		};
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
