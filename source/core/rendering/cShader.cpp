#include "cShader.h"

#include <format>
#include <glad/glad.h>

#include "core/filesystem/cFileSystem.h"
#include "core/log/Log.h"

namespace df
{
    cShader::cShader( const std::string& _name )
    : m_program( 0 )
    {
        const unsigned vertex   = compileShader( std::format( "{}_vertex.glsl", _name ), GL_VERTEX_SHADER );
        const unsigned fragment = compileShader( std::format( "{}_fragment.glsl", _name ), GL_FRAGMENT_SHADER );

        m_program = glCreateProgram();
        glAttachShader( m_program, vertex );
        glAttachShader( m_program, fragment );
        glLinkProgram( m_program );

        int success;
        glGetProgramiv( m_program, GL_LINK_STATUS, &success );

        if( success )
            LOG_MESSAGE( std::format( "Successfully linked shader program: {}", _name ) );
        else
        {
            char info_log[ 512 ];
            glGetProgramInfoLog( m_program, 512, nullptr, info_log );
            LOG_ERROR( std::format( "Failed to link shader program: {} - {}", _name, info_log ) );
        }

        glDeleteShader( vertex );
        glDeleteShader( fragment );
    }

    cShader::~cShader()
    {
        glDeleteProgram( m_program );
    }

    void cShader::use() const
    {
        glUseProgram( m_program );
    }

    void cShader::setUniformBool( const std::string& _name, const bool& _value ) const
    {
        glUniform1i( glGetUniformLocation( m_program, _name.data() ), _value );
    }

    void cShader::setUniformInt( const std::string& _name, const int& _value ) const
    {
        glUniform1i( glGetUniformLocation( m_program, _name.data() ), _value );
    }

    void cShader::setUniformFloat( const std::string& _name, const float& _value ) const
    {
        glUniform1f( glGetUniformLocation( m_program, _name.data() ), _value );
    }

    unsigned cShader::compileShader( const std::string& _name, const int& _type )
    {
        const std::string shader_string = filesystem::readContent( _name, "\n" );
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
