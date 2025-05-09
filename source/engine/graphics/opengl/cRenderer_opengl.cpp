#include "cRenderer_opengl.h"

#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <SDL3/SDL_init.h>

#include "assets/cQuad_opengl.h"
#include "assets/cTexture_opengl.h"
#include "callbacks/cDefaultQuad_opengl.h"
#include "cFramebuffer_opengl.h"
#include "engine/graphics/window/WindowTypes.h"
#include "engine/managers/cEventManager.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/profiling/ProfilingMacros.h"
#include "engine/profiling/ProfilingMacros_opengl.h"
#include "functions/sTextureImage.h"
#include "graphics/cRenderer.h"
#include "imgui_impl_sdl3.h"
#include "managers/assets/cCameraManager.h"
#include "OpenGlTypes.h"
#include "window/cWindow_opengl.h"

namespace df::opengl
{
	cRenderer_opengl::cRenderer_opengl( const std::string& _window_name )
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

		DF_ProfilingGPUContext;
		window->setViewport();

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
		ImGui_ImplOpenGL3_Init( "#version 430 core" );
	}

	void cRenderer_opengl::initializeDeferred()
	{
		DF_ProfilingScopeCpu;
		DF_ProfilingScopeGpu;

		m_deferred_screen_quad                  = new cQuad_opengl( "deferred", cVector3f( m_window->getSize() / 2, 0 ), m_window->getSize() );
		m_deferred_screen_quad->render_callback = new cRenderCallback( "deferred_quad_final", "deferred_quad_final", render_callbacks::cDefaultQuad_opengl::deferredQuadFinal );

		m_deferred_framebuffer = new cFramebuffer_opengl( "deferred", 3, true, m_window->getSize() );

		cTexture_opengl* texture = reinterpret_cast< cTexture_opengl* >( m_deferred_framebuffer->render_textues[ 0 ] );
		texture->bind();
		sTextureImage::set2D( texture, 0, sTextureImage::sInternalFormat::Sized::kRGB16F, m_window->getSize(), 0, sTextureImage::sFormat::kRGB, kFloat, nullptr );
		texture = reinterpret_cast< cTexture_opengl* >( m_deferred_framebuffer->render_textues[ 1 ] );
		texture->bind();
		sTextureImage::set2D( texture, 0, sTextureImage::sInternalFormat::Base::kRGB, m_window->getSize(), 0, sTextureImage::sFormat::kRGB, kFloat, nullptr );

		texture = reinterpret_cast< cTexture_opengl* >( m_deferred_framebuffer->render_textues[ 2 ] );
		texture->bind();
		sTextureImage::set2D( texture, 0, sTextureImage::sInternalFormat::Base::kRGB, m_window->getSize(), 0, sTextureImage::sFormat::kRGB, kUnsignedByte, nullptr );

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
