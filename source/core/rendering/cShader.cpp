#include "cShader.h"

#include <format>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "core/filesystem/cFileSystem.h"
#include "core/log/Log.h"

namespace df
{
    cShader::cShader( const std::string& _name )
    : m_program( 0 )
    {
        const unsigned vertex   = compileShader( std::format( "data/shaders/{}_vertex.glsl", _name ), GL_VERTEX_SHADER );
        const unsigned fragment = compileShader( std::format( "data/shaders/{}_fragment.glsl", _name ), GL_FRAGMENT_SHADER );

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

    void cShader::setUniform1B( const std::string& _name, const bool& _value ) const
    {
        glUniform1i( glGetUniformLocation( m_program, _name.data() ), _value );
    }

    void cShader::setUniform1I( const std::string& _name, const int& _value ) const
    {
        glUniform1i( glGetUniformLocation( m_program, _name.data() ), _value );
    }

    void cShader::setUniform1F( const std::string& _name, const float& _value ) const
    {
        glUniform1f( glGetUniformLocation( m_program, _name.data() ), _value );
    }

    void cShader::setUniformMatrix4Fv( const std::string& _name, const glm::mat4& _matrix, const int& _amount, const bool& _transpose ) const
    {
        glUniformMatrix4fv( glGetUniformLocation( m_program, _name.data() ), _amount, _transpose, value_ptr( _matrix ) );
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
