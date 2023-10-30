#include "cApplication.h"

#include <stb_image.h>
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <map>
#include <freetype/freetype.h>

#include "core/filesystem/cFileSystem.h"
#include "core/managers/cCameraManager.h"
#include "core/managers/cEventManager.h"
#include "core/managers/cInputManager.h"
#include "core/managers/cQuadManager.h"
#include "core/managers/cRenderCallbackManager.h"
#include "core/misc/cTimer.h"
#include "core/profiling/Profiling.h"
#include "core/rendering/cRenderer.h"
#include "core/rendering/assets/cQuad.h"
#include "core/rendering/assets/cTexture.h"
#include "core/rendering/cameras/cFreeFlightCamera.h"

cApplication::cApplication()
{
#if defined ( DEBUG )
    PROFILING_SCOPE( __FUNCTION__ );
#endif

    initialize();

    stbi_set_flip_vertically_on_load( true );
    df::cRenderer::initialize();
    df::cEventManager::initialize();
    df::cInputManager::initialize();
    df::cRenderCallbackManager::initialize();
    df::cQuadManager::initialize();
    df::cCameraManager::initialize();
}

cApplication::~cApplication()
{
#if defined ( DEBUG )
    PROFILING_BEGIN( __FUNCTION__ );
#endif

    df::cCameraManager::deinitialize();
    df::cQuadManager::deinitialize();
    df::cRenderCallbackManager::deinitialize();
    df::cInputManager::deinitialize();
    df::cEventManager::deinitialize();
    df::cRenderer::deinitialize();

#if defined ( DEBUG )
    PROFILING_END;
#endif

    df::profiling::printClear();
    df::memory::printLeaks();
}

void cApplication::run()
{
#if defined ( DEBUG )
    PROFILING_SCOPE( __FUNCTION__ );
#endif

    glEnable( GL_DEPTH_TEST );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    df::cFreeFlightCamera flight_camera( 1, .1f );
    df::cCamera           camera2d( df::cCamera::kOrthographic, df::color::white, 90, 0 );

    df::cQuad quad( glm::vec3( 0, 0, 0 ), glm::vec2( .5f, .5f ), df::color::blue, "data/textures/wall.jpg" );

    FT_Library library;
    if( FT_Init_FreeType( &library ) )
    {
        LOG_ERROR( "Failed to initialize FreeType library" );
        return;
    }

    FT_Face face;
    if( FT_New_Face( library, "data/fonts/MontserratMedium.ttf", 0, &face ) )
    {
        LOG_ERROR( "Failed to load font" );
        return;
    }

    FT_Set_Pixel_Sizes( face, 0, 48 );

    struct sCharacter
    {
        glm::vec2    scale;
        glm::ivec2   size;
        glm::ivec2   bearing;
        unsigned int advance;
    };

    std::map< char, sCharacter > characters;

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    df::cTexture texture_array( GL_TEXTURE_2D_ARRAY );
    texture_array.bind();
    texture_array.setTextureParameterI( GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    texture_array.setTextureParameterI( GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    texture_array.setTextureParameterI( GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    texture_array.setTextureParameterI( GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage3D( GL_TEXTURE_2D_ARRAY, 0, GL_RED, 60, 48, 95, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr );

    for( unsigned char c = 32; c < 127; ++c )
    {
        if( FT_Load_Char( face, c, FT_LOAD_RENDER ) )
        {
            LOG_ERROR( "Failed to load glyph" );
            continue;
        }

        sCharacter character = {};
        character.scale      = glm::vec2( static_cast< float >( face->glyph->bitmap.width ) / 60.f, static_cast< float >( face->glyph->bitmap.rows ) / 48.f );
        character.size       = glm::ivec2( face->glyph->bitmap.width, face->glyph->bitmap.rows );
        character.bearing    = glm::ivec2( face->glyph->bitmap_left, face->glyph->bitmap_top );
        character.advance    = face->glyph->advance.x;

        glTexSubImage3D( GL_TEXTURE_2D_ARRAY, 0, 0, 0, c - 32, character.size.x, character.size.y, 1, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer );

        characters.insert( std::pair< char, sCharacter >( c, character ) );
    }

    texture_array.unbind();
    FT_Done_Face( face );
    FT_Done_FreeType( library );

    unsigned int vao, vbo;
    glGenVertexArrays( 1, &vao );
    glGenBuffers( 1, &vbo );
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * 6 * 4, nullptr, GL_DYNAMIC_DRAW );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof( float ), nullptr );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );

    df::cShader text_shader( "default_text" );

    df::cRenderer::resizeWindow();
    df::cRenderer::setCursorInputMode( GLFW_CURSOR_DISABLED );

    while( !glfwWindowShouldClose( df::cRenderer::getWindow() ) )
    {
#if defined ( DEBUG )
        PROFILING_SCOPE( __FUNCTION__"::loop" );
#endif

        df::cInputManager::update();
        // df::cEventManager::invoke( df::event::update, m_timer.getDeltaSecond() );

        float delta_time = static_cast< float >( m_timer.getDeltaSecond() );
        flight_camera.update( delta_time );

        flight_camera.beginRender( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        quad.render();
        flight_camera.endRender();

        camera2d.update();

        camera2d.beginRender( GL_DEPTH_BUFFER_BIT );

        text_shader.use();
        text_shader.setUniform4F( "u_color", df::color::orange );
        text_shader.setUniformMatrix4F( "u_projection_view", camera2d.projection_view );

        texture_array.bind();

        glActiveTexture( GL_TEXTURE0 );
        glBindVertexArray( vao );

        std::string                 text( "Testing" );
        float                       x     = 50;
        float                       y     = 50;
        float                       scale = 1;
        std::string::const_iterator c;
        for( c = text.begin(); c != text.end(); ++c )
        {
            sCharacter ch = characters[ *c ];

            float xpos = x + ch.bearing.x * scale;
            float ypos = y - ( ch.size.y - ch.bearing.y ) * scale;

            float w = ch.size.x * scale;
            float h = ch.size.y * scale;

            float vertices[ 6 ][ 4 ] = {
                { xpos, ypos + h, 0.0f, 0.0f },
                { xpos, ypos, 0.0f, ch.scale.y },
                { xpos + w, ypos, ch.scale.x, ch.scale.y },

                { xpos, ypos + h, 0.0f, 0.0f },
                { xpos + w, ypos, ch.scale.x, ch.scale.y },
                { xpos + w, ypos + h, ch.scale.x, 0.0f }
            };

            text_shader.setUniform1I( "u_layer", *c - 32 );
            glBindBuffer( GL_ARRAY_BUFFER, vbo );
            glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( vertices ), vertices );
            glBindBuffer( GL_ARRAY_BUFFER, 0 );

            glDrawArrays( GL_TRIANGLES, 0, 6 );

            x += ( ch.advance >> 6 ) * scale;
        }
        glBindVertexArray( 0 );
        glBindTexture( GL_TEXTURE_2D, 0 );

        camera2d.endRender();

        df::cRenderer::render();
    }
}

void cApplication::initialize()
{
#if defined( DEBUG )
    PROFILING_SCOPE( __FUNCTION__ );

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

    df::filesystem::remove( "log.txt" );
    df::filesystem::remove( "memory.txt" );
    df::filesystem::remove( "profiling.txt" );

    LOG_RAW( "Starting DragonForge-Engine" );
}
