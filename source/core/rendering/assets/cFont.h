#pragma once
#include <map>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "core/misc/cColor.h"
#include "core/misc/Misc.h"

namespace df
{
    class cTexture;
    struct iRenderCallback;

    class cFont
    {
    public:
        DISABLE_COPY_AND_MOVE( cFont )

        struct sCharacter
        {
            glm::vec2    scale;
            glm::ivec2   size;
            glm::ivec2   bearing;
            unsigned int advance;
        };

        explicit cFont( const std::string& _file = {} );
        ~cFont();

        void load( const std::string& _file );

        void render( const std::string& _text, const glm::vec3& _position, const glm::vec2& _size, const cColor& _color = color::white );

        void bindTexture( const int& _index = 0 ) const;

        cColor           color;
        unsigned         vertex_array_object;
        unsigned         vertex_buffer_object;
        iRenderCallback* render_callback;

        std::map< char, sCharacter > characters;

    protected:
        cTexture* m_texture_array;

        unsigned m_ebo;
    };
}
