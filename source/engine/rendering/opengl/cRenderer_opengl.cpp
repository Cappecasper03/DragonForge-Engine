#include "cRenderer_opengl.h"

#include <format>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <tracy/TracyOpenGL.hpp>

#include "callbacks/DefaultQuadCB_opengl.h"
#include "cFramebuffer_opengl.h"
#include "engine/filesystem/cFileSystem.h"
#include "engine/managers/cEventManager.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "framework/application/cApplication.h"

namespace df::opengl
{
	cRenderer_opengl::cRenderer_opengl()
	{
		ZoneScoped;

		if( m_glfw_use_count == 0 )
		{
			glfwInit();
			DF_LOG_MESSAGE( "Initialized GLFW" );
		}
		m_glfw_use_count++;

		glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
		glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_API );
		glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

		m_window = glfwCreateWindow( m_window_size.x, m_window_size.y, cApplication::getName().c_str(), nullptr, nullptr );
		if( !m_window )
		{
			DF_LOG_ERROR( "Failed to create window" );
			return;
		}
		DF_LOG_MESSAGE( std::format( "Created window [{}, {}]", m_window_size.x, m_window_size.y ) );

		glfwMakeContextCurrent( m_window );
		glfwSwapInterval( 0 );

		if( !gladLoadGLLoader( reinterpret_cast< GLADloadproc >( glfwGetProcAddress ) ) )
		{
			DF_LOG_ERROR( "Failed to initialize GLAD" );
			return;
		}
		DF_LOG_MESSAGE( "Initialized GLAD" );

		glViewport( 0, 0, m_window_size.x, m_window_size.y );

		glfwSetFramebufferSizeCallback( m_window, framebufferSizeCallback );

		TracyGpuContext;
		if( m_use_deferred )
			initializeDeferred();

#ifdef DEBUG
		glEnable( GL_DEBUG_OUTPUT );
		glDebugMessageCallback( debugMessageCallback, nullptr );
#endif
	}

	cRenderer_opengl::~cRenderer_opengl()
	{
		ZoneScoped;

		if( m_window )
			glfwDestroyWindow( m_window );

		m_glfw_use_count--;
		if( m_glfw_use_count == 0 )
		{
			glfwTerminate();
			DF_LOG_MESSAGE( "Deinitialized GLFW" );
		}
	}

	void cRenderer_opengl::render()
	{
		ZoneScoped;

		if( m_use_deferred )
		{
			m_framebuffer->bind();

			cEventManager::invoke( event::render_3d );
			cEventManager::invoke( event::render_2d );

			m_framebuffer->unbind();

			cCamera* camera = cCameraManager::get( "default_2d" );
			camera->beginRender( GL_DEPTH_BUFFER_BIT );

			m_screen_quad->render();

			camera->endRender();
		}
		else
		{
			cEventManager::invoke( event::render_3d );
			cEventManager::invoke( event::render_2d );
		}

		glfwSwapBuffers( m_window );
		TracyGpuCollect;
	}

	void cRenderer_opengl::clearBuffers( const int _buffers, const cColor& _color )
	{
		ZoneScoped;

		glClearColor( _color.r, _color.g, _color.b, _color.a );
		glClear( _buffers );
	}

	void cRenderer_opengl::resizeWindow( const int _width, const int _height )
	{
		ZoneScoped;

		if( _width > 0 && _height > 0 )
		{
			glfwSetWindowSize( m_window, _width, _height );
			return;
		}

		cEventManager::invoke( event::on_window_resize, m_window_size.x, m_window_size.y );
	}

	void cRenderer_opengl::setCursorInputMode( const int _value )
	{
		ZoneScoped;

		glfwSetInputMode( m_window, GLFW_CURSOR, _value );
	}

	void cRenderer_opengl::initializeDeferred()
	{
		ZoneScoped;

		m_screen_quad                  = new cQuad_opengl( "deferred", glm::vec3( m_window_size / 2, 0 ), glm::vec2( m_window_size ) );
		m_screen_quad->render_callback = cRenderCallbackManager::create( "default_quad_deferred", render_callback::defaultQuadDeferred );

		m_framebuffer = new cFramebuffer_opengl( "deferred", 3 );

		cTexture_opengl* texture = reinterpret_cast< cTexture_opengl* >( m_framebuffer->render_textues[ 0 ] );
		texture->bind();
		texture->setTexImage2D( 0, GL_RGB16F, m_window_size.x, m_window_size.y, 0, GL_RGB, GL_FLOAT, nullptr );

		texture = reinterpret_cast< cTexture_opengl* >( m_framebuffer->render_textues[ 1 ] );
		texture->bind();
		texture->setTexImage2D( 0, GL_RGB, m_window_size.x, m_window_size.y, 0, GL_RGB, GL_FLOAT, nullptr );

		texture = reinterpret_cast< cTexture_opengl* >( m_framebuffer->render_textues[ 2 ] );
		texture->bind();
		texture->setTexImage2D( 0, GL_RGBA, m_window_size.x, m_window_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr );

		texture->unbind();
	}

	void cRenderer_opengl::framebufferSizeCallback( GLFWwindow* /*_window*/, const int _width, const int _height )
	{
		ZoneScoped;

		glViewport( 0, 0, _width, _height );

		if( _width == 0 || _height == 0 )
			return;

		cEventManager::invoke( event::on_window_resize, _width, _height );
	}

	void cRenderer_opengl::debugMessageCallback( unsigned _source, unsigned _type, unsigned _id, unsigned _severity, int /*_length*/, const char* _message, const void* /*_user_param*/ )
	{
		ZoneScoped;

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
				source = "None";
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
				type = "None";
			};
		}

		switch( _severity )
		{
			case GL_DEBUG_SEVERITY_HIGH:
			{
				DF_LOG_ERROR( std::format( "OpenGL\n"
				                           "Source: {}\n"
				                           "Type: {}\n"
				                           "ID: {}\n"
				                           "Severity: High\n"
				                           "Message: {}",
				                           source,
				                           type,
				                           _id,
				                           _message ) );
			}
			break;
			case GL_DEBUG_SEVERITY_MEDIUM:
			{
				DF_LOG_WARNING( std::format( "OpenGL\n"
				                             "Source: {}\n"
				                             "Type: {}\n"
				                             "ID: {}\n"
				                             "Severity: Medium\n"
				                             "Message: {}",
				                             source,
				                             type,
				                             _id,
				                             _message ) );
			}
			break;
			case GL_DEBUG_SEVERITY_LOW:
			{
				DF_LOG_WARNING( std::format( "OpenGL\n"
				                             "Source: {}\n"
				                             "Type: {}\n"
				                             "ID: {}\n"
				                             "Severity: Low\n"
				                             "Message: {}",
				                             source,
				                             type,
				                             _id,
				                             _message ) );
			}
			break;
			default:
			{
				DF_LOG_MESSAGE( std::format( "OpenGL\n"
				                             "Source: {}\n"
				                             "Type: {}\n"
				                             "ID: {}\n"
				                             "Severity: None\n"
				                             "Message: {}",
				                             source,
				                             type,
				                             _id,
				                             _message ) );
			};
		}
	}
}
