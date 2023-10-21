#include "cApplication.h"

#include <format>
#include <stb_image.h>
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/filesystem/cFileSystem.h"
#include "core/managers/cEventManager.h"
#include "core/managers/cInputManager.h"
#include "core/misc/cTimer.h"
#include "core/profiling/Profiling.h"
#include "core/rendering/cShader.h"
#include "core/rendering/camera/cCamera.h"
#include "core/rendering/camera/cFreeFlightCamera.h"

cApplication::cApplication()
: m_window( nullptr )
{
    initialize();
    initializeOpenGL();

    df::cEventManager::initialize();
    df::cInputManager::initialize( m_window );
}

cApplication::~cApplication()
{
    df::profiling::printClear();
    df::memory::printLeaks();

    df::cInputManager::deinitialize();
    df::cEventManager::deinitialize();

    glfwTerminate();
    LOG_MESSAGE( "Deinitialized GLFW" );
}

void cApplication::run()
{
    
    const df::cShader test( "test" );

    unsigned texture;
    glGenTextures( 1, &texture );

    glBindTexture( GL_TEXTURE_2D, texture );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    int            width, height, nr_channels;
    unsigned char* data = stbi_load( "data/textures/wall.jpg", &width, &height, &nr_channels, 0 );
    if( data )
    {
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
        glGenerateMipmap( GL_TEXTURE_2D );
    }
    else
        LOG_WARNING( "Failed to load texture: wall.jpg" );

    stbi_image_free( data );

    constexpr float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    // constexpr unsigned int indices[ ]  = { 0, 1, 3, 1, 2, 3 };

    unsigned vbo, vao, ebo;
    glGenVertexArrays( 1, &vao );
    glGenBuffers( 1, &vbo );
    glGenBuffers( 1, &ebo );

    glBindVertexArray( vao );

    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

    // glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );
    // glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), static_cast< void* >( 0 ) );
    glEnableVertexAttribArray( 0 );

    // glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), reinterpret_cast< void* >( 3 * sizeof( float ) ) );
    // glEnableVertexAttribArray( 1 );

    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), reinterpret_cast< void* >( 3* sizeof( float ) ) );
    glEnableVertexAttribArray( 1 );

    glm::mat4 model = glm::mat4( 1 );
    model           = rotate( model, glm::radians( -55.f ), glm::vec3( 1, 0, 0 ) );

    glEnable( GL_DEPTH_TEST );

    df::cFreeFlightCamera camera( 5, .1f );

    int window_width, window_height;
    glfwGetWindowSize( m_window, &window_width, &window_height );
    df::cEventManager::invoke( df::event::on_window_resize, window_width, window_height );

    while( !glfwWindowShouldClose( m_window ) )
    {
        float delta_time = static_cast< float >( m_timer.getDeltaSecond() );
        camera.update( delta_time );

        df::cInputManager::getInstance()->update();
        // df::cEventManager::invoke( df::event::update, m_timer.getDeltaSecond() );
        // df::cEventManager::invoke( df::event::render_3d );
        // df::cEventManager::invoke( df::event::render_2d );

        camera.beginRender( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        glBindTexture( GL_TEXTURE_2D, texture );

        model = rotate( model, delta_time * glm::radians( 50.f ), glm::vec3( .5f, 1, 0 ) );

        test.use();

        test.setUniformMatrix4Fv( "u_model", model );
        test.setUniformMatrix4Fv( "u_view", camera.view );
        test.setUniformMatrix4Fv( "u_projection", camera.projection );

        // glBindVertexArray( vao );
        // glDrawElements( GL_TRIANGLES, 6,GL_UNSIGNED_INT, 0 );
        glDrawArrays( GL_TRIANGLES, 0, 36 );

        glfwSwapBuffers( m_window );
    }
}

void cApplication::framebufferSizeCallback( GLFWwindow* /*_window*/, const int _width, const int _height )
{
    glViewport( 0, 0, _width, _height );

    df::cEventManager::invoke( df::event::on_window_resize, _width, _height );
}

void cApplication::initialize()
{
#if defined( DEBUG )
    AllocConsole();
    FILE* file;
    freopen_s( &file, "CONOUT$", "w", stdout );
    SetConsoleTitle( L"DragonForge-Engine Logs" );
#endif

    size_t  size;
    wchar_t wbuffer[ MAX_PATH ];
    char    buffer[ MAX_PATH ];

    GetModuleFileName( nullptr, wbuffer, MAX_PATH );
    wcstombs_s( &size, buffer, MAX_PATH, wbuffer, MAX_PATH );

    const std::string executable_path( buffer );
    df::filesystem::setExecutableDirectory( executable_path.substr( 0, executable_path.find_last_of( '\\' ) + 1 ) );

    m_name = executable_path.substr( executable_path.find_last_of( '\\' ) + 1 );
    m_name.erase( m_name.length() - 4 );

    df::filesystem::remove( "logs.txt" );
    df::filesystem::remove( "memory.txt" );
    df::filesystem::remove( "profiling.txt" );

    LOG_RAW( "Starting DragonForge-Engine" );
}

void cApplication::initializeOpenGL()
{
    glfwInit();
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    LOG_MESSAGE( "Initialized GLFW" );

    constexpr int window_width  = 1200;
    constexpr int window_height = 800;
    m_window                    = glfwCreateWindow( window_width, window_height, m_name.c_str(), nullptr, nullptr );
    if( !m_window )
    {
        LOG_ERROR( "Failed to create window" );
        return;
    }
    LOG_MESSAGE( std::format("Created window [{}, {}]" , window_width, window_height ) );

    glfwMakeContextCurrent( m_window );

    if( !gladLoadGLLoader( reinterpret_cast< GLADloadproc >( glfwGetProcAddress ) ) )
    {
        LOG_ERROR( "Failed to initialize GLAD" );
        return;
    }
    LOG_MESSAGE( "Initialized GLAD" );

    glViewport( 0, 0, window_width, window_height );

    glfwSetFramebufferSizeCallback( m_window, framebufferSizeCallback );
}
