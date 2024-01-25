#pragma once

#include <string>
#include <vector>

#include "core/misc/Misc.h"

namespace df
{
    class cTexture;

    class cFramebuffer
    {
    public:
        DISABLE_COPY_AND_MOVE( cFramebuffer )

        explicit cFramebuffer( std::string _name, const unsigned& _num_render_textures = 0, const bool& _generate_render_buffer = false );
        ~cFramebuffer();

        static void setFramebufferTexture2D( int _attachment, int _tex_target, int _texture, int _level );

        void        bind() const;
        static void unbind();

        std::vector< cTexture* > render_textues;
        const std::string        name;

    private:
        unsigned m_buffer;
        unsigned m_render_buffer;
    };
}
