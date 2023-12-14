#pragma once
#include <map>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "iRenderAsset.h"
#include "core/misc/cColor.h"
#include "core/misc/Misc.h"
#include "core/rendering/cShader.h"

namespace df
{
    class cTexture;
    struct iRenderCallback;

    class cFont : public iAsset
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

        explicit cFont( std::string _name, const std::string& _file = {} );
        ~cFont() override;

        bool load( const std::string& _file );

        void render( const std::string& _text, const glm::vec3& _position, const glm::vec2& _scale = { 1, 1 }, const cColor& _color = color::white );

        void bindTexture( const int& _index = 0 ) const;

        // TODO: Make into struct "TextData"
        const std::string&                  getLatestText() const { return m_latest_text; }
        const glm::vec3&                    getLatestPosition() const { return m_latest_position; }
        const glm::vec2&                    getLatestScale() const { return m_latest_scale; }
        const cColor&                       getLatestColor() const { return m_latest_color; }
        const std::map< char, sCharacter >& getCharacters() const { return m_characters; }

        unsigned         vertex_array_object;
        unsigned         vertex_buffer_object;
        iRenderCallback* render_callback;

    protected:
        cTexture* m_texture_array;
        unsigned  m_ebo;

        std::string                  m_latest_text;
        glm::vec3                    m_latest_position;
        glm::vec2                    m_latest_scale;
        cColor                       m_latest_color;
        std::map< char, sCharacter > m_characters;
    };
}
