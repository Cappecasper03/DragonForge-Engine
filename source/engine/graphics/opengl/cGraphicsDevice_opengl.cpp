#include "cGraphicsDevice_opengl.h"

#include <clay.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <SDL3/SDL_init.h>

#include "assets/cQuad_opengl.h"
#include "assets/textures/cTexture2D_opengl.h"
#include "callbacks/cDefaultQuad_opengl.h"
#include "engine/graphics/api/iFramebuffer.h"
#include "engine/graphics/api/iGraphicsDevice.h"
#include "engine/graphics/assets/textures/iSampler.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/opengl/buffers/cFrameBuffer_opengl.h"
#include "engine/graphics/types/sSamplerParameter.h"
#include "engine/graphics/types/sSceneUniforms.h"
#include "engine/graphics/window/WindowTypes.h"
#include "engine/managers/cCameraManager.h"
#include "engine/managers/cEventManager.h"
#include "engine/managers/cLightManager.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/profiling/ProfilingMacros.h"
#include "engine/profiling/ProfilingMacros_opengl.h"
#include "imgui_impl_sdl3.h"
#include "OpenGlTypes.h"
#include "window/cWindow_opengl.h"

namespace df::opengl
{
	cGraphicsDevice_opengl::cGraphicsDevice_opengl( const std::string& _window_name )
		: m_vertex_scene_buffer( cBuffer_opengl::kUniform, false )
		, m_fragment_scene_buffer( cBuffer_opengl::kUniform, false )
		, m_vertex_array_gui( false )
		, m_vertex_buffer_gui( cBuffer_opengl::kVertex, false )
		, m_index_buffer_gui( cBuffer_opengl::kIndex, false )
		, m_push_constant_gui( cBuffer_opengl::kUniform, false )
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

		const std::vector< unsigned > indices = { 0, 1, 2, 3, 4, 5 };

		m_shader_gui.load( "clay" );
		m_vertex_array_gui.generate();
		m_vertex_buffer_gui.generate();
		m_index_buffer_gui.generate();

		m_vertex_array_gui.bind();

		m_index_buffer_gui.bind();
		m_index_buffer_gui.setData( sizeof( unsigned ) * 6, indices.data(), cBuffer_opengl::kStaticDraw );

		m_vertex_array_gui.setAttribute( 0, 1, kUnsignedInt, sizeof( sVertexGui ), offsetof( sVertexGui, sVertexGui::vertex_id ) );
		m_vertex_array_gui.unbind();

		m_push_constant_gui.generate();
		m_push_constant_gui.bind();
		m_push_constant_gui.setData( sizeof( sPushConstantsGui ), nullptr, cBuffer_opengl::kDynamicDraw );
		m_push_constant_gui.unbind();

#ifdef DF_Debug
		glEnable( GL_DEBUG_OUTPUT );
		glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
		glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true );
		glDebugMessageCallback( debugMessageCallback, nullptr );
#endif
	}

	cGraphicsDevice_opengl::~cGraphicsDevice_opengl()
	{
		DF_ProfilingScopeCpu;
		DF_ProfilingScopeGpu;

		delete m_sampler_linear;

		if( ImGui::GetCurrentContext() )
		{
			ImGui_ImplSDL3_Shutdown();
			ImGui_ImplOpenGL3_Shutdown();
			ImGui::DestroyContext();
		}

		if( cRenderer::isDeferred() )
		{
			delete m_deferred_render_buffer;
			delete m_deferred_framebuffer;
			delete m_deferred_screen_quad->m_render_callback;
			delete m_deferred_screen_quad;
		}

		delete m_window;
	}

	void cGraphicsDevice_opengl::render()
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

			m_deferred_framebuffer->unbind();

			cCamera* camera = cCameraManager::get( "default_2d" );
			camera->beginRender( cCamera::kDepth );

			m_deferred_screen_quad->render();

			camera->endRender();
		}
		else
			cEventManager::invoke( event::render_3d );

		iGraphicsDevice::renderGui();

		if( ImGui::GetCurrentContext() )
		{
			DF_ProfilingScopeNamedCpu( "ImGui" );
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

	void cGraphicsDevice_opengl::beginRendering( const int _clear_buffers, const cColor& _color )
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

	void cGraphicsDevice_opengl::initialize()
	{
		DF_ProfilingScopeCpu;

		m_sampler_linear = iSampler::create();
		m_sampler_linear->addParameter( sSamplerParameter::kMinFilter, sSamplerParameter::kLinear );
		m_sampler_linear->addParameter( sSamplerParameter::kMagFilter, sSamplerParameter::kLinear );
		m_sampler_linear->addParameter( sSamplerParameter::kWrapS, sSamplerParameter::kRepeat );
		m_sampler_linear->addParameter( sSamplerParameter::kWrapT, sSamplerParameter::kRepeat );
		m_sampler_linear->update();

		m_sampler_linear->bind();
	}

	void cGraphicsDevice_opengl::initializeImGui()
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

	void cGraphicsDevice_opengl::renderGui( const sPushConstantsGui& _push_constants, const cTexture2D* _texture )
	{
		DF_ProfilingScopeCpu;
		DF_ProfilingScopeGpu;

		m_shader_gui.use();

		m_push_constant_gui.bind();
		m_push_constant_gui.setSubData( 0, sizeof( sPushConstantsGui ), &_push_constants );
		m_push_constant_gui.unbind();
		m_push_constant_gui.bindBase( 0 );

		if( _texture )
			_texture->bind();

		glEnable( kBlend );
		glBlendFunc( kSrcAlpha, kOneMinusSrcAlpha );

		m_vertex_array_gui.bind();
		glDrawElements( kTriangles, 6, kUnsignedInt, nullptr );
	}

	void cGraphicsDevice_opengl::initializeDeferred()
	{
		DF_ProfilingScopeCpu;
		DF_ProfilingScopeGpu;

		m_deferred_screen_quad                    = new cQuad_opengl( "deferred", cVector3f( m_window->getSize() / 2, 0 ), m_window->getSize() );
		m_deferred_screen_quad->m_render_callback = new cRenderCallback( "deferred_quad_final", "deferred_quad_final", render_callbacks::cDefaultQuad_opengl::deferredQuadFinal );

		m_deferred_framebuffer   = new cFrameBuffer_opengl();
		m_deferred_render_buffer = new cRenderBuffer_opengl();

		m_deferred_framebuffer->bind();
		m_deferred_render_buffer->bind();
		m_deferred_render_buffer->setStorage( GL_DEPTH_STENCIL, m_window->getSize() );
		reinterpret_cast< cFrameBuffer_opengl* >( m_deferred_framebuffer )->setRenderBuffer( GL_DEPTH_STENCIL_ATTACHMENT, *m_deferred_render_buffer );

		cTexture2D::sDescription description{
			.name       = "framebuffer_texture",
			.size       = m_window->getSize(),
			.mip_levels = 1,
			.format     = sTextureFormat::kRGB,
			.usage      = sTextureUsage::kColorAttachment,
		};
		cTexture2D_opengl* texture = reinterpret_cast< cTexture2D_opengl* >( cTexture2D::create( description ) );
		texture->bind();
		reinterpret_cast< cFrameBuffer_opengl* >( m_deferred_framebuffer )->setTexture2D( 0, texture );
		m_deferred_framebuffer->m_render_textures.push_back( texture );

		description.format = sTextureFormat::kRGB16sf;
		texture            = reinterpret_cast< cTexture2D_opengl* >( cTexture2D::create( description ) );
		texture->bind();
		reinterpret_cast< cFrameBuffer_opengl* >( m_deferred_framebuffer )->setTexture2D( 1, texture );
		m_deferred_framebuffer->m_render_textures.push_back( texture );

		description.format = sTextureFormat::kRGB;
		texture            = reinterpret_cast< cTexture2D_opengl* >( cTexture2D::create( description ) );
		texture->bind();
		reinterpret_cast< cFrameBuffer_opengl* >( m_deferred_framebuffer )->setTexture2D( 2, texture );
		m_deferred_framebuffer->m_render_textures.push_back( texture );

		texture->unbind();
	}

	void cGraphicsDevice_opengl::debugMessageCallback( unsigned _source,
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
				break;
			}
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
				break;
			}
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
				break;
			}
			default:
			{
				break;
			}
		}
	}
}
