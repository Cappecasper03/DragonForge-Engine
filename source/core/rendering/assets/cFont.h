#pragma once

#include <unordered_map>
#include <glm/vec2.hpp>

#include "cTexture.h"
#include "core/misc/Misc.h"

namespace df
{
    class cFont
    {
    public:
        DISABLE_COPY_AND_MOVE( cFont )

        explicit cFont( const std::string& _font_file = {} );
        ~cFont();

        bool load( const std::string& _font_file );

        void render( const std::string& _text );

        cTexture* getBitmap() const { return m_bitmap; }

    private:
        struct sCharacter
        {
            glm::ivec2 size;
            glm::ivec2 bearing;
        };

        std::unordered_map< char, sCharacter > m_characters;
        cTexture*                              m_bitmap;
    };
}
