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

        void setUniformBool( const std::string& _name, const bool& _value ) const;
        void setUniformInt( const std::string& _name, const int& _value ) const;
        void setUniformFloat( const std::string& _name, const float& _value ) const;

        void setUniformSampler( const std::string& _name, const int& _sampler ) const { setUniformInt( _name, _sampler ); }

    private:
        static unsigned compileShader( const std::string& _name, const int& _type );

        unsigned m_program;
    };
}
