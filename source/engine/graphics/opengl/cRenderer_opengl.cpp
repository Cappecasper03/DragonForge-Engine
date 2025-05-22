#include "cRenderer_opengl.h"

#include <clay.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <SDL3/SDL_init.h>

#include "engine/graphics/clay-imgui-renderer.h"

#include "assets/cQuad_opengl.h"
#include "assets/cTexture_opengl.h"
#include "callbacks/cDefaultQuad_opengl.h"
#include "engine/graphics/api/iFramebuffer.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/opengl/buffers/cFrameBuffer_opengl.h"
#include "engine/graphics/types/sSceneUniforms.h"
#include "engine/graphics/window/WindowTypes.h"
#include "engine/managers/cCameraManager.h"
#include "engine/managers/cEventManager.h"
#include "engine/managers/cLightManager.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/profiling/ProfilingMacros.h"
#include "engine/profiling/ProfilingMacros_opengl.h"
#include "functions/sTextureImage.h"
#include "functions/sTextureParameter.h"
#include "imgui_impl_sdl3.h"
#include "OpenGlTypes.h"
#include "window/cWindow_opengl.h"

namespace df::opengl
{
	const Clay_Color COLOR_ORANGE = { .88f, .55f, .19f, 1 };

	Clay_ElementDeclaration sidebarItemConfig = { .layout = { .sizing = { .width = CLAY_SIZING_GROW( 0 ), .height = CLAY_SIZING_FIXED( 50 ) } }, .backgroundColor = COLOR_ORANGE };

	// Re-useable components are just normal functions
	void SidebarItemComponent()
	{
		CLAY( sidebarItemConfig )
		{
			// children go here...
		}
	}

	cRenderer_opengl::cRenderer_opengl( const std::string& _window_name )
		: m_vertex_scene_buffer( cBuffer_opengl::kUniform, false )
		, m_fragment_scene_buffer( cBuffer_opengl::kUniform, false )
	{
		DF_ProfilingScopeCpu;

		cWindow_opengl* window = new cWindow_opengl();
		m_window               = window;

		m_window->create( _window_name, window::kOpenGl | window::kResizable );
		cWindow_opengl::setSwapInterval( cWindow_opengl::kImmediate );

		if( !gladLoadGLLoader( reinterpret_cast< GLADloadproc >( SDL_GL_GetProcAddress ) ) )
		{
			DF_LogError( "Failed to initialize GLAD" );
			return;
		}
		DF_LogMessage( "Initialized GLAD" );

		DF_ProfilingGpuContext;
		window->setViewport();

		if( cRenderer::isDeferred() )
			initializeDeferred();

		m_vertex_scene_buffer.generate();
		m_vertex_scene_buffer.bind();
		m_vertex_scene_buffer.setData( sizeof( sVertexSceneUniforms ), nullptr, cBuffer_opengl::kDynamicDraw );
		m_vertex_scene_buffer.unbind();

		m_fragment_scene_buffer.generate();
		m_fragment_scene_buffer.bind();
		m_fragment_scene_buffer.setData( sizeof( sLight ) * cLightManager::m_max_lights + sizeof( unsigned ), nullptr, cBuffer_opengl::kDynamicDraw );
		m_fragment_scene_buffer.unbind();

#ifdef DF_Debug
		glEnable( GL_DEBUG_OUTPUT );
		glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
		glDebugMessageCallback( debugMessageCallback, nullptr );
#endif

		{
			const uint64_t   memory = Clay_MinMemorySize();
			const Clay_Arena arean{
				.capacity = memory,
				.memory   = static_cast< char* >( std::malloc( memory ) ),
			};

			Clay_Initialize( arean, Clay_Dimensions( m_window->getSize().height(), m_window->getSize().width() ), Clay_ErrorHandler() );
		}
	}

	cRenderer_opengl::~cRenderer_opengl()
	{
		DF_ProfilingScopeCpu;
		DF_ProfilingScopeGpu;

		if( ImGui::GetCurrentContext() )
		{
			ImGui_ImplSDL3_Shutdown();
			ImGui_ImplOpenGL3_Shutdown();
			ImGui::DestroyContext();
		}

		if( cRenderer::isDeferred() )
		{
			delete m_deferred_framebuffer;
			delete m_deferred_screen_quad->m_render_callback;
			delete m_deferred_screen_quad;
		}

		delete m_window;
	}

	void cRenderer_opengl::render()
	{
		DF_ProfilingScopeCpu;
		DF_ProfilingScopeGpu;

		if( m_window_minimized )
			return;

		if( m_window_resized )
		{
			m_window->updateSize();
			reinterpret_cast< cWindow_opengl* >( m_window )->setViewport();
			cEventManager::invoke( event::on_window_resize, m_window->getSize().x(), m_window->getSize().y() );
			m_window_resized = false;
			return;
		}

		if( cRenderer::isDeferred() )
		{
			m_deferred_framebuffer->bind();

			cEventManager::invoke( event::render_3d );
			cEventManager::invoke( event::render_2d );

			m_deferred_framebuffer->unbind();

			cCamera* camera = cCameraManager::get( "default_2d" );
			camera->beginRender( cCamera::kDepth );

			m_deferred_screen_quad->render();

			camera->endRender();
		}
		else
		{
			cEventManager::invoke( event::render_3d );
			cEventManager::invoke( event::render_2d );
		}

		{
			const Clay_Color COLOR_LIGHT = { .87f, .84f, .82f, 1 };
			const Clay_Color COLOR_RED   = { .65f, .25f, .1f, 1 };

			Clay_SetLayoutDimensions( { static_cast< float >( m_window->getSize().width() ), static_cast< float >( m_window->getSize().height() ) } );

			Clay_BeginLayout();

			CLAY( {
				.id              = CLAY_ID( "OuterContainer" ),
				.layout          = { .sizing = { CLAY_SIZING_GROW( 0, 0 ), CLAY_SIZING_GROW( 0, 0 ) }, .padding = CLAY_PADDING_ALL( 16 ), .childGap = 16 },
				.backgroundColor = { .98f, .98f, 1, 1 },
			} )
			{
				CLAY( {
						.id              = CLAY_ID( "SideBar" ),
						.layout          = { .sizing          = { .width = CLAY_SIZING_FIXED( 300 ), .height = CLAY_SIZING_GROW( 0, 0 ) },
                                            .padding         = CLAY_PADDING_ALL( 16 ),
                                            .childGap        = 16,
                                            .layoutDirection = CLAY_TOP_TO_BOTTOM,
						},
						.backgroundColor = COLOR_LIGHT,
						.cornerRadius    = {
									.topLeft     = .5f,
									.topRight    = .1f,
									.bottomLeft  = .1f,
									.bottomRight = .1f,
									}
                    } )
				{
					CLAY( {
						.id              = CLAY_ID( "ProfilePictureOuter" ),
						.layout          = { .sizing         = { .width = CLAY_SIZING_GROW( 0, 0 ) },
                                            .padding        = CLAY_PADDING_ALL( 16 ),
                                            .childGap       = 16,
                                            .childAlignment = { .y = CLAY_ALIGN_Y_CENTER } },
						.backgroundColor = COLOR_RED,
						.cornerRadius    = {
									.topLeft     = 1,
									.topRight    = .1f,
									.bottomLeft  = .1f,
									.bottomRight = .1f,
									},
						.border          = {
							.color = { 0, 1, 0, 1, },
							.width = {
												 .left            = 1,
												 .right           = 1,
												 .top             = 1,
												 .bottom          = 1,
												 .betweenChildren = 0,
                                    },
						}
                    } )
					{
						CLAY( {
							.id     = CLAY_ID( "ProfilePicture" ),
							.layout = { .sizing = { .width = CLAY_SIZING_FIXED( 60 ), .height = CLAY_SIZING_FIXED( 60 ) } },
						} )
						{}
						CLAY_TEXT( CLAY_STRING( "Clay - UI Library" ),
						           CLAY_TEXT_CONFIG( {
									   .textColor = { 1, 1, 1, 1 },
									   .fontSize  = 12,
                        } ) );
					}

					// Standard C code like loops etc work inside components
					for( int i = 0; i < 5; i++ )
					{
						SidebarItemComponent();
					}

					CLAY( {
						.id              = CLAY_ID( "MainContent" ),
						.layout          = { .sizing = { .width = CLAY_SIZING_GROW( 0, 0 ), .height = CLAY_SIZING_GROW( 0, 0 ) } },
						.backgroundColor = COLOR_LIGHT,
					} )
					{}
				}
			}

			Clay_RenderCommandArray render_commands = Clay_EndLayout();

			static cCamera camera( "clay", cCamera::eType::kOrthographic, color::white, 90.f, -1.f, 100.f );
			camera.m_flip_y = true;
			resizeWindow();
			camera.beginRender( cCamera::kDepth );

			for( int i = 0; i < render_commands.length; ++i )
			{
				Clay_RenderCommand& command = render_commands.internalArray[ i ];

				switch( command.commandType )
				{
					case CLAY_RENDER_COMMAND_TYPE_RECTANGLE:
					{
						struct sVertex
						{
							cVector2f position;
							cColor    color;
							cVector2f uv;
							cVector2f size;
							cVector4f corner_radii;
							cVector4f border_widths;
							float     is_border;
						};

						static cShader_opengl      shader( "clay" );
						static cVertexArray_opengl vertex_array;
						static cBuffer_opengl      vertex_buffer( cBuffer_opengl::kVertex );
						static bool                first = true;

						if( first )
						{
							vertex_array.bind();

							vertex_buffer.bind();
							vertex_buffer.setData( sizeof( sVertex ) * 6, nullptr, cBuffer_opengl::kDynamicDraw );

							vertex_array.setAttribute( 0, 2, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::position ) );
							vertex_array.setAttribute( 1, 4, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::color ) );
							vertex_array.setAttribute( 2, 2, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::uv ) );
							vertex_array.setAttribute( 3, 2, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::size ) );
							vertex_array.setAttribute( 4, 4, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::corner_radii ) );
							vertex_array.setAttribute( 5, 4, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::border_widths ) );
							vertex_array.setAttribute( 6, 1, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::is_border ) );
							vertex_array.unbind();

							first = false;
						}

						const float x = command.boundingBox.x;
						const float y = command.boundingBox.y;
						const float w = command.boundingBox.width;
						const float h = command.boundingBox.height;

						const float     min_dim_half   = std::min( w, h ) * 0.5f;
						const cVector4f radiiForShader = cVector4f( command.renderData.rectangle.cornerRadius.topLeft * min_dim_half,
						                                            command.renderData.rectangle.cornerRadius.topRight * min_dim_half,
						                                            command.renderData.rectangle.cornerRadius.bottomLeft * min_dim_half,
						                                            command.renderData.rectangle.cornerRadius.bottomRight * min_dim_half );

						sVertex vertices[ 6 ];

						vertices[ 0 ].position = cVector2f( x, y );
						vertices[ 0 ].uv       = cVector2f( 0, 0 );

						vertices[ 1 ].position = cVector2f( x + w, y );
						vertices[ 1 ].uv       = cVector2f( 1, 0 );

						vertices[ 2 ].position = cVector2f( x, y + h );
						vertices[ 2 ].uv       = cVector2f( 0, 1 );

						vertices[ 3 ].position = cVector2f( x + w, y );
						vertices[ 3 ].uv       = cVector2f( 1, 0 );

						vertices[ 4 ].position = cVector2f( x + w, y + h );
						vertices[ 4 ].uv       = cVector2f( 1, 1 );

						vertices[ 5 ].position = cVector2f( x, y + h );
						vertices[ 5 ].uv       = cVector2f( 0, 1 );

						for( int k = 0; k < 6; ++k )
						{
							vertices[ k ].color.r = command.renderData.rectangle.backgroundColor.r;
							vertices[ k ].color.g = command.renderData.rectangle.backgroundColor.g;
							vertices[ k ].color.b = command.renderData.rectangle.backgroundColor.b;
							vertices[ k ].color.a = command.renderData.rectangle.backgroundColor.a;

							vertices[ k ].size         = cVector2f( w, h );
							vertices[ k ].corner_radii = radiiForShader;
							vertices[ k ].is_border    = 0;
						}

						shader.use();

						vertex_array.bind();

						vertex_buffer.bind();
						vertex_buffer.setData( sizeof( sVertex ) * 6, &vertices, cBuffer_opengl::kDynamicDraw );

						glDrawArrays( kTriangles, 0, 6 );

						break;
					}
					case CLAY_RENDER_COMMAND_TYPE_BORDER:
					{
						struct sVertex
						{
							cVector2f position;
							cColor    color;
							cVector2f uv;
							cVector2f size;
							cVector4f corner_radii;
							cVector4f border_widths;
							float     is_border;
						};

						static cShader_opengl      shader( "clay" );
						static cVertexArray_opengl vertex_array;
						static cBuffer_opengl      vertex_buffer( cBuffer_opengl::kVertex );
						static bool                first = true;

						if( first )
						{
							vertex_array.bind();

							vertex_buffer.bind();
							vertex_buffer.setData( sizeof( sVertex ) * 6, nullptr, cBuffer_opengl::kDynamicDraw );

							vertex_array.setAttribute( 0, 2, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::position ) );
							vertex_array.setAttribute( 1, 4, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::color ) );
							vertex_array.setAttribute( 2, 2, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::uv ) );
							vertex_array.setAttribute( 3, 2, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::size ) );
							vertex_array.setAttribute( 4, 4, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::corner_radii ) );
							vertex_array.setAttribute( 5, 4, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::border_widths ) );
							vertex_array.setAttribute( 6, 1, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::is_border ) );
							vertex_array.unbind();

							first = false;
						}

						const float x = command.boundingBox.x;
						const float y = command.boundingBox.y;
						const float w = command.boundingBox.width;
						const float h = command.boundingBox.height;

						const float     min_dim_half   = std::min( w, h ) * 0.5f;
						const cVector4f radiiForShader = cVector4f( command.renderData.border.cornerRadius.topLeft * min_dim_half,
						                                            command.renderData.border.cornerRadius.topRight * min_dim_half,
						                                            command.renderData.border.cornerRadius.bottomLeft * min_dim_half,
						                                            command.renderData.border.cornerRadius.bottomRight * min_dim_half );

						sVertex vertices[ 6 ];

						vertices[ 0 ].position = cVector2f( x, y );
						vertices[ 0 ].uv       = cVector2f( 0, 0 );

						vertices[ 1 ].position = cVector2f( x + w, y );
						vertices[ 1 ].uv       = cVector2f( 1, 0 );

						vertices[ 2 ].position = cVector2f( x, y + h );
						vertices[ 2 ].uv       = cVector2f( 0, 1 );

						vertices[ 3 ].position = cVector2f( x + w, y );
						vertices[ 3 ].uv       = cVector2f( 1, 0 );

						vertices[ 4 ].position = cVector2f( x + w, y + h );
						vertices[ 4 ].uv       = cVector2f( 1, 1 );

						vertices[ 5 ].position = cVector2f( x, y + h );
						vertices[ 5 ].uv       = cVector2f( 0, 1 );

						for( int k = 0; k < 6; ++k )
						{
							vertices[ k ].color.r = command.renderData.border.color.r;
							vertices[ k ].color.g = command.renderData.border.color.g;
							vertices[ k ].color.b = command.renderData.border.color.b;
							vertices[ k ].color.a = command.renderData.border.color.a;

							vertices[ k ].size         = cVector2f( w, h );
							vertices[ k ].corner_radii = radiiForShader;

							vertices[ k ].border_widths.x() = command.renderData.border.width.left;
							vertices[ k ].border_widths.y() = command.renderData.border.width.right;
							vertices[ k ].border_widths.z() = command.renderData.border.width.top;
							vertices[ k ].border_widths.w() = command.renderData.border.width.bottom;

							vertices[ k ].is_border = 1;
						}

						shader.use();

						vertex_array.bind();

						vertex_buffer.bind();
						vertex_buffer.setData( sizeof( sVertex ) * 6, &vertices, cBuffer_opengl::kDynamicDraw );

						glDrawArrays( kTriangles, 0, 6 );

						break;
					}
					case CLAY_RENDER_COMMAND_TYPE_TEXT:
					{
						break;
					}
					case CLAY_RENDER_COMMAND_TYPE_IMAGE:
					{
						break;
					}
					case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START:
					{
						break;
					}
					case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END:
					{
						break;
					}
				}
			}

			camera.endRender();
		}

		if( ImGui::GetCurrentContext() )
		{
			DF_ProfilingScopeNamesCpu( "ImGui" );
			DF_ProfilingScopeNamedGpu( imgui, "ImGui" );

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL3_NewFrame();
			ImGui::NewFrame();

			cEventManager::invoke( event::imgui );

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
		}

		reinterpret_cast< cWindow_opengl* >( m_window )->swap();
		DF_ProfilingCollectGpu;
	}

	void cRenderer_opengl::beginRendering( const int _clear_buffers, const cColor& _color )
	{
		DF_ProfilingScopeCpu;
		DF_ProfilingScopeGpu;

		const int color = _clear_buffers & cCamera::eClearBuffer::kColor ? GL_COLOR_BUFFER_BIT : 0;
		const int depth = _clear_buffers & cCamera::eClearBuffer::kDepth ? GL_DEPTH_BUFFER_BIT : 0;

		glClearColor( _color.r, _color.g, _color.b, _color.a );
		glClear( color | depth );

		{
			const cCamera* camera = cCameraManager::getInstance()->m_current;

			const sVertexSceneUniforms uniforms{
				.view_projection = camera->m_view_projection,
			};

			m_vertex_scene_buffer.bind();
			m_vertex_scene_buffer.setSubData( 0, sizeof( uniforms ), &uniforms );
			m_vertex_scene_buffer.unbind();
			m_vertex_scene_buffer.bindBase( 1 );
		}

		{
			const std::vector< sLight >& lights      = cLightManager::getLights();
			const unsigned               light_count = static_cast< unsigned >( lights.size() );

			const size_t     current_lights_size = sizeof( sLight ) * lights.size();
			constexpr size_t full_lights_size    = sizeof( sLight ) * cLightManager::m_max_lights;

			m_fragment_scene_buffer.bind();
			m_fragment_scene_buffer.setSubData( 0, current_lights_size, lights.data() );
			m_fragment_scene_buffer.setSubData( full_lights_size, sizeof( light_count ), &light_count );
			m_fragment_scene_buffer.unbind();
			m_fragment_scene_buffer.bindBase( 2 );
		}
	}

	void cRenderer_opengl::initializeImGui()
	{
		DF_ProfilingScopeCpu;
		DF_ProfilingScopeGpu;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io     = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		ImGui_ImplSDL3_InitForOpenGL( m_window->getWindow(), reinterpret_cast< cWindow_opengl* >( m_window )->getContext() );
		ImGui_ImplOpenGL3_Init( "#version 450 core" );
	}

	void cRenderer_opengl::initializeDeferred()
	{
		DF_ProfilingScopeCpu;
		DF_ProfilingScopeGpu;

		m_deferred_screen_quad                    = new cQuad_opengl( "deferred", cVector3f( m_window->getSize() / 2, 0 ), m_window->getSize() );
		m_deferred_screen_quad->m_render_callback = new cRenderCallback( "deferred_quad_final", "deferred_quad_final", render_callbacks::cDefaultQuad_opengl::deferredQuadFinal );

		m_deferred_framebuffer = new cFrameBuffer_opengl();

		cTexture_opengl* texture = new cTexture_opengl( "", cTexture_opengl::k2D );
		texture->bind();
		sTextureParameter::setInteger( texture, sTextureParameter::kMinFilter, sTextureParameter::sMinFilter::kNearest );
		sTextureParameter::setInteger( texture, sTextureParameter::kMagFilter, sTextureParameter::sMagFilter::kNearest );
		sTextureImage::set2D( texture, 0, sTextureImage::sInternalFormat::Base::kRGB, m_window->getSize(), 0, sTextureImage::sFormat::kRGB, kUnsignedInt, nullptr );
		reinterpret_cast< cFrameBuffer_opengl* >( m_deferred_framebuffer )->setTexture2D( 0, texture );
		m_deferred_framebuffer->m_render_textures.push_back( texture );

		texture = new cTexture_opengl( "", cTexture_opengl::k2D );
		texture->bind();
		sTextureParameter::setInteger( texture, sTextureParameter::kMinFilter, sTextureParameter::sMinFilter::kNearest );
		sTextureParameter::setInteger( texture, sTextureParameter::kMagFilter, sTextureParameter::sMagFilter::kNearest );
		sTextureImage::set2D( texture, 0, sTextureImage::sInternalFormat::Sized::kRGB16F, m_window->getSize(), 0, sTextureImage::sFormat::kRGB, kFloat, nullptr );
		reinterpret_cast< cFrameBuffer_opengl* >( m_deferred_framebuffer )->setTexture2D( 1, texture );
		m_deferred_framebuffer->m_render_textures.push_back( texture );

		texture = new cTexture_opengl( "", cTexture_opengl::k2D );
		texture->bind();
		sTextureParameter::setInteger( texture, sTextureParameter::kMinFilter, sTextureParameter::sMinFilter::kNearest );
		sTextureParameter::setInteger( texture, sTextureParameter::kMagFilter, sTextureParameter::sMagFilter::kNearest );
		sTextureImage::set2D( texture, 0, sTextureImage::sInternalFormat::Base::kRGB, m_window->getSize(), 0, sTextureImage::sFormat::kRGB, kFloat, nullptr );
		reinterpret_cast< cFrameBuffer_opengl* >( m_deferred_framebuffer )->setTexture2D( 2, texture );
		m_deferred_framebuffer->m_render_textures.push_back( texture );

		texture->unbind();
	}

	void cRenderer_opengl::debugMessageCallback( unsigned _source,
	                                             unsigned _type,
	                                             unsigned _id,
	                                             unsigned _severity,
	                                             int /*_length*/,
	                                             const char* _message,
	                                             const void* /*_user_param*/ )
	{
		DF_ProfilingScopeCpu;

		std::string source;
		switch( _source )
		{
			case GL_DEBUG_SOURCE_API:
			{
				source = "API";
			}
			break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			{
				source = "Window System";
			}
			break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER:
			{
				source = "Shader Compiler";
			}
			break;
			case GL_DEBUG_SOURCE_THIRD_PARTY:
			{
				source = "Third Party";
			}
			break;
			case GL_DEBUG_SOURCE_APPLICATION:
			{
				source = "Application";
			}
			break;
			case GL_DEBUG_SOURCE_OTHER:
			{
				source = "Other";
			}
			break;
			default:
			{
			}
		}

		std::string type;
		switch( _type )
		{
			case GL_DEBUG_TYPE_ERROR:
			{
				type = "Error";
			}
			break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			{
				type = "Deprecated Behavior";
			}
			break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			{
				type = "Undefined Behavior";
			}
			break;
			case GL_DEBUG_TYPE_PORTABILITY:
			{
				type = "Portability";
			}
			break;
			case GL_DEBUG_TYPE_PERFORMANCE:
			{
				type = "Performance";
			}
			break;
			case GL_DEBUG_TYPE_MARKER:
			{
				type = "Marker";
			}
			break;
			case GL_DEBUG_TYPE_PUSH_GROUP:
			{
				type = "Push Group";
			}
			break;
			case GL_DEBUG_TYPE_POP_GROUP:
			{
				type = "Pop Group";
			}
			break;
			case GL_DEBUG_TYPE_OTHER:
			{
				type = "Other";
			}
			break;
			default:
			{
			}
		}

		std::string message = _message;
		std::ranges::replace( message, '\n', ' ' );
		switch( _severity )
		{
			case GL_DEBUG_SEVERITY_HIGH:
			{
				DF_LogError( fmt::format( "OpenGL, "
				                          "Source: {}, "
				                          "Type: {}, "
				                          "ID: {}, "
				                          "Severity: High, "
				                          "Message: {}",
				                          source,
				                          type,
				                          _id,
				                          message ) );
			}
			break;
			case GL_DEBUG_SEVERITY_MEDIUM:
			{
				DF_LogWarning( fmt::format( "OpenGL, "
				                            "Source: {}, "
				                            "Type: {}, "
				                            "ID: {}, "
				                            "Severity: Medium, "
				                            "Message: {}",
				                            source,
				                            type,
				                            _id,
				                            message ) );
			}
			break;
			case GL_DEBUG_SEVERITY_LOW:
			{
				DF_LogWarning( fmt::format( "OpenGL, "
				                            "Source: {}, "
				                            "Type: {}, "
				                            "ID: {}, "
				                            "Severity: Low, "
				                            "Message: {}",
				                            source,
				                            type,
				                            _id,
				                            message ) );
			}
			break;
			default:
			{
			}
		}
	}
}
