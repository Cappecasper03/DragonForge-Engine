#pragma once

#include <map>
#include <string>

#include "core/misc/Misc.h"

namespace df
{
    class cShader
    {
    public:
        DISABLE_COPY_AND_MOVE( cShader );

        explicit cShader( const std::string& _name );
        ~cShader();

        void use() const;

        void setUniformBool( const std::string& _name, const bool& _value );
        void setUniformInt( const std::string& _name, const int& _value );
        void setUniformFloat( const std::string& _name, const float& _value );

        void setUniformSampler( const std::string& _name, const int& _value ) { setUniformInt( _name, _value ); }

        int  getUniformLocation( const std::string& _name );
        void addUniformLocation( const std::string& _name );

    private:
        static unsigned compileShader( const std::string& _name, const int& _type );

        unsigned m_shader_program;

        std::map< std::string, int > m_uniform_locations;
    };
}
