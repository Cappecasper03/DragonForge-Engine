#include "iGraphicsDevice.h"

#include <fmt/format.h>

#include "engine/graphics/cameras/cCamera.h"
#include "engine/graphics/window/iWindow.h"
#include "engine/managers/cEventManager.h"
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
	}

	void iGraphicsDevice::renderGui()
	{
		DF_ProfilingScopeCpu;

		Clay_BeginLayout();

		cEventManager::invoke( event::render_gui );

		const Clay_RenderCommandArray command_array = Clay_EndLayout();

		static cCamera camera( "clay", cCamera::eType::kOrthographic, color::white, 90.f, -1.f, 100.f );
		camera.m_flip_y = true;
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

					for( int k = 0; k < 6; ++k )
					{
						vertices[ k ].color.r = command.renderData.rectangle.backgroundColor.r;
						vertices[ k ].color.g = command.renderData.rectangle.backgroundColor.g;
						vertices[ k ].color.b = command.renderData.rectangle.backgroundColor.b;
						vertices[ k ].color.a = command.renderData.rectangle.backgroundColor.a;

						vertices[ k ].size          = cVector2f( w, h );
						vertices[ k ].corner_radius = radius;
						vertices[ k ].type          = kRectangle;
					}

					renderGuiRectangle( vertices );
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

					for( int k = 0; k < 6; ++k )
					{
						vertices[ k ].color.r = command.renderData.border.color.r;
						vertices[ k ].color.g = command.renderData.border.color.g;
						vertices[ k ].color.b = command.renderData.border.color.b;
						vertices[ k ].color.a = command.renderData.border.color.a;

						vertices[ k ].size          = cVector2f( w, h );
						vertices[ k ].corner_radius = radius;

						vertices[ k ].border_widths.x() = command.renderData.border.width.left;
						vertices[ k ].border_widths.y() = command.renderData.border.width.right;
						vertices[ k ].border_widths.z() = command.renderData.border.width.top;
						vertices[ k ].border_widths.w() = command.renderData.border.width.bottom;

						vertices[ k ].type = kBorder;
					}

					renderGuiBorder( vertices );
					break;
				}
				case CLAY_RENDER_COMMAND_TYPE_TEXT:
				{
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

					for( int k = 0; k < 6; ++k )
					{
						vertices[ k ].color.r = command.renderData.image.backgroundColor.r;
						vertices[ k ].color.g = command.renderData.image.backgroundColor.g;
						vertices[ k ].color.b = command.renderData.image.backgroundColor.b;
						vertices[ k ].color.a = command.renderData.image.backgroundColor.a;

						vertices[ k ].size          = cVector2f( w, h );
						vertices[ k ].corner_radius = radius;
						vertices[ k ].type          = kImage;
					}

					renderGuiImage( vertices, static_cast< const iTexture* >( command.renderData.image.imageData ) );
					break;
				}
			}
		}

		camera.endRender();
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
