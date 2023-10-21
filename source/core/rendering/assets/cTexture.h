#pragma once

#include <string>

#include "core/misc/Misc.h"

namespace df
{
    class cTexture
    {
    public:
        DISABLE_COPY_AND_MOVE( cTexture );

        explicit cTexture( const int& _target );
        virtual  ~cTexture();

        void load( const std::string& _file, const int& _mipmaps = 0, const bool& _generate_mipmaps = true ) const;

        void setTextureParameterI( const int& _name, const int& _param ) const;

        void bind() const;
        void unbind() const;

    protected:
        unsigned m_texture;
        int      m_target;
    };
}
