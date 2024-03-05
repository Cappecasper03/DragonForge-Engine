#pragma once

#include <string>

#include "engine/misc/Misc.h"
#include "engine/rendering/iFramebuffer.h"

namespace df::opengl
{
    class cFramebuffer : public iFramebuffer
    {
    public:
        DF_DISABLE_COPY_AND_MOVE( cFramebuffer )

        explicit cFramebuffer( std::string _name, unsigned _num_render_textures = 0, bool _generate_render_buffer = false );
        ~cFramebuffer() override;

        static void setFramebufferTexture2D( int _attachment, int _tex_target, int _texture, int _level );

        void bind() override;
        void unbind() override;

    private:
        unsigned m_buffer;
        unsigned m_render_buffer;
    };
}
