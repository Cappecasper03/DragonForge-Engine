#pragma once

#include <string>
#include <glm/vec3.hpp>

#include "iRenderAsset.h"
#include "engine/misc/cColor.h"

namespace df
{
    class cTexture;

    class cQuad : public iRenderAsset
    {
    public:
        DF_DISABLE_COPY_AND_MOVE( cQuad );

        cQuad( std::string _name, const glm::vec3& _position, const glm::vec2& _size, const cColor& _color = color::white, const std::string& _texture_file = {} );
        ~cQuad() override;

        void render() override;

        cTexture* texture;
        cColor    color;

    protected:
        void initOpenGL( const std::string& _texture_file );
        void initVulkan( const std::string& _texture_file );

        struct sVertex
        {
            glm::vec3 position;
            glm::vec2 tex_coord;
        };

        sVertex  m_vertices[ 4 ];
        unsigned m_indices[ 6 ];

        static bool m_initialized_once;
    };
}
