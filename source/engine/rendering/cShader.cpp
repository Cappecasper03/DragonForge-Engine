#include "cShader.h"

#include <format>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "engine/filesystem/cFileSystem.h"
#include "engine/log/Log.h"

#if PROFILING
#include "engine/profiling/Profiling.h"
#endif

namespace df
{
    cShader::cShader( std::string _name )
    : name( std::move( _name ) ),
      m_program( 0 )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        const unsigned vertex   = compileShader( std::format( "{}_vertex.glsl", name ), GL_VERTEX_SHADER );
        const unsigned fragment = compileShader( std::format( "{}_fragment.glsl", name ), GL_FRAGMENT_SHADER );

        m_program = glCreateProgram();
        glAttachShader( m_program, vertex );
        glAttachShader( m_program, fragment );
        glLinkProgram( m_program );

        int success;
        glGetProgramiv( m_program, GL_LINK_STATUS, &success );

        if( success )
            LOG_MESSAGE( std::format( "Successfully linked shader program: {}", name ) );
        else
        {
            char info_log[ 512 ];
            glGetProgramInfoLog( m_program, 512, nullptr, info_log );
            LOG_ERROR( std::format( "Failed to link shader program: {} - {}", name, info_log ) );
        }

        glDeleteShader( vertex );
        glDeleteShader( fragment );
    }

    cShader::~cShader()
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        glDeleteProgram( m_program );
    }

    void cShader::use() const
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        glUseProgram( m_program );
    }

    void cShader::setUniform1B( const std::string& _name, const bool& _value ) const
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        glUniform1i( glGetUniformLocation( m_program, _name.data() ), _value );
    }

    void cShader::setUniform1I( const std::string& _name, const int& _value ) const
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        glUniform1i( glGetUniformLocation( m_program, _name.data() ), _value );
    }

    void cShader::setUniform1F( const std::string& _name, const float& _value ) const
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        glUniform1f( glGetUniformLocation( m_program, _name.data() ), _value );
    }

    void cShader::setUniform4F( const std::string& _name, const glm::vec4& _vector ) const
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        glUniform4f( glGetUniformLocation( m_program, _name.data() ), _vector.x, _vector.y, _vector.z, _vector.w );
    }

    void cShader::setUniform4F( const std::string& _name, const cColor& _color ) const
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        glUniform4f( glGetUniformLocation( m_program, _name.data() ), _color.r, _color.g, _color.b, _color.a );
    }

    void cShader::setUniformMatrix4F( const std::string& _name, const glm::mat4& _matrix, const int& _amount, const bool& _transpose ) const
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        glUniformMatrix4fv( glGetUniformLocation( m_program, _name.data() ), _amount, _transpose, value_ptr( _matrix ) );
    }

    unsigned cShader::compileShader( const std::string& _name, const int& _type )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        const std::string shader_string = filesystem::readContent( "binaries/shaders/" + _name, "\n" );
        const char*       shader        = shader_string.data();

        const unsigned shader_id = glCreateShader( _type );
        glShaderSource( shader_id, 1, &shader, nullptr );
        glCompileShader( shader_id );

        int success;
        glGetShaderiv( shader_id, GL_COMPILE_STATUS, &success );

        if( success )
            LOG_MESSAGE( std::format( "Successfully compiled shader: {}", _name ) );
        else
        {
            char info_log[ 512 ];
            glGetShaderInfoLog( shader_id, 512, nullptr, info_log );
            LOG_ERROR( std::format( "Failed to compile shader: {} - {}", _name, info_log ) );
        }

        return shader_id;
    }
}
