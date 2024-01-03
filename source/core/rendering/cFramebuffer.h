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

        explicit cFramebuffer( std::string _name, const unsigned& _num_render_textures, const bool& _generate_textures = true, const bool& _generate_render_buffer = true );
        ~cFramebuffer();

        void bind() const;

        std::vector< cTexture* > render_textues;
        const std::string        name;

    private:
        unsigned m_buffer;
        unsigned m_render_buffer;
    };
}
