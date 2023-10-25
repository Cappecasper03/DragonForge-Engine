#pragma once

#include <unordered_map>
#include <glm/vec2.hpp>

#include "cTexture.h"
#include "iRenderAsset.h"
#include "core/misc/cColor.h"
#include "core/misc/Misc.h"

namespace df
{
    class cFont : public iRenderAsset
    {
    public:
        DISABLE_COPY_AND_MOVE( cFont )

        explicit cFont( const std::string& _font_file = {} );
        ~cFont();

        bool load( const std::string& _font_file );

        void render( const glm::vec2& _position, const std::string& _text, const glm::vec2& _scale = glm::vec2( 1 ), const cColor& _color = color::white );

        cTexture* getBitmap() const { return m_bitmap; }

    private:
        struct sCharacter
        {
            glm::ivec2 size;
            glm::ivec2 bearing;
            unsigned   advance;
        };

        struct sVertex
        {
            glm::vec3 position;
            glm::vec2 tex_coord;
        };

        std::unordered_map< char, sCharacter > m_characters;
        cTexture*                              m_bitmap;

        sVertex  m_vertices[ 4 ];
        unsigned m_indices[ 6 ];
    };
}
