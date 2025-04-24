#include "cRenderer_opengl.h"

#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <SDL3/SDL_init.h>

#include "assets/cTexture_opengl.h"
#include "callbacks/DefaultQuadCB_opengl.h"
#include "cFramebuffer_opengl.h"
#include "engine/filesystem/cFileSystem.h"
#include "engine/managers/cEventManager.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/profiling/ProfilingMacros.h"
#include "engine/profiling/ProfilingMacros_opengl.h"
#include "imgui_impl_sdl3.h"

namespace df::opengl
{
	cRenderer_opengl::cRenderer_opengl( const std::string& _window_name )
	{
		DF_ProfilingScopeCpu;

		SDL_Init( SDL_INIT_VIDEO );
		DF_LogMessage( "Initialized SDL" );

		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 6 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

		m_window = SDL_CreateWindow( _window_name.data(), m_window_size.x(), m_window_size.y(), SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );
		if( !m_window )
		{
			DF_LogError( "Failed to create window" );
			return;
		}
		DF_LogMessage( fmt::format( "Created window [{}, {}]", m_window_size.x(), m_window_size.y() ) );

		m_context = SDL_GL_CreateContext( m_window );
		SDL_GL_SetSwapInterval( 0 );

		if( !gladLoadGLLoader( reinterpret_cast< GLADloadproc >( SDL_GL_GetProcAddress ) ) )
		{
			DF_LogError( "Failed to initialize GLAD" );
			return;
		}
		DF_LogMessage( "Initialized GLAD" );

		DF_ProfilingGPUContext;
		glViewport( 0, 0, m_window_size.x(), m_window_size.y() );

		if( cRenderer::isDeferred() )
			initializeDeferred();

#ifdef DF_Debug
		glEnable( GL_DEBUG_OUTPUT );
		glDebugMessageCallback( debugMessageCallback, nullptr );
#endif
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
			delete m_deferred_screen_quad->render_callback;
			delete m_deferred_screen_quad;
		}

		if( m_context )
			SDL_GL_DestroyContext( m_context );

		if( m_window )
			SDL_DestroyWindow( m_window );

		SDL_Quit();
		DF_LogMessage( "Quit SDL" );
	}

	void cRenderer_opengl::render()
	{
		DF_ProfilingScopeCpu;
		DF_ProfilingScopeGpu;

		if( m_window_minimized )
			return;

		if( m_window_resized )
		{
			int width = 0, height = 0;
			SDL_GetWindowSize( m_window, &width, &height );
			glViewport( 0, 0, width, height );
			cEventManager::invoke( event::on_window_resize, width, height );
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
			camera->beginRender( cCamera::eDepth );

			m_deferred_screen_quad->render();

			camera->endRender();
		}
		else
		{
			cEventManager::invoke( event::render_3d );
			cEventManager::invoke( event::render_2d );
		}

		if( ImGui::GetCurrentContext() )
		{
			DF_ProfilingScopeNamedGPU( imgui, __FUNCTION__ "::ImGui" );
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL3_NewFrame();
			ImGui::NewFrame();
			cEventManager::invoke( event::imgui );
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
		}

		SDL_GL_SwapWindow( m_window );
		DF_ProfilingCollectGpu;
	}

	void cRenderer_opengl::beginRendering( const int _clear_buffers, const cColor& _color )
	{
		DF_ProfilingScopeCpu;
		DF_ProfilingScopeGpu;

		const int color = _clear_buffers & cCamera::eClearBuffer::eColor ? GL_COLOR_BUFFER_BIT : 0;
		const int depth = _clear_buffers & cCamera::eClearBuffer::eDepth ? GL_DEPTH_BUFFER_BIT : 0;

		glClearColor( _color.r, _color.g, _color.b, _color.a );
		glClear( color | depth );
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

		ImGui_ImplSDL3_InitForOpenGL( m_window, m_context );
		ImGui_ImplOpenGL3_Init( "#version 460" );
	}

	void cRenderer_opengl::initializeDeferred()
	{
		DF_ProfilingScopeCpu;
		DF_ProfilingScopeGpu;

		m_deferred_screen_quad                  = new cQuad_opengl( "deferred", cVector3f( m_window_size / 2, 0 ), cVector2f( m_window_size ) );
		m_deferred_screen_quad->render_callback = new cRenderCallback( "deferred_quad_final", "deferred_quad_final", render_callback::deferredQuadFinal );

		m_deferred_framebuffer = new cFramebuffer_opengl( "deferred", 3, true, m_window_size );

		cTexture_opengl* texture = reinterpret_cast< cTexture_opengl* >( m_deferred_framebuffer->render_textues[ 0 ] );
		texture->bind();
		texture->setTexImage2D( 0, GL_RGB16F, m_window_size.x(), m_window_size.y(), 0, GL_RGB, GL_FLOAT, nullptr );

		texture = reinterpret_cast< cTexture_opengl* >( m_deferred_framebuffer->render_textues[ 1 ] );
		texture->bind();
		texture->setTexImage2D( 0, GL_RGB, m_window_size.x(), m_window_size.y(), 0, GL_RGB, GL_FLOAT, nullptr );

		texture = reinterpret_cast< cTexture_opengl* >( m_deferred_framebuffer->render_textues[ 2 ] );
		texture->bind();
		texture->setTexImage2D( 0, GL_RGBA, m_window_size.x(), m_window_size.y(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr );

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
				                          _message ) );
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
				                            _message ) );
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
				                            _message ) );
			}
			break;
			default:
			{
			}
		}
	}
}
