#pragma once

#include <glad/glad.h>

#include "engine/managers/assets/cCameraManager.h"
#include "engine/rendering/cShader.h"
#include "engine/rendering/assets/cFont.h"

namespace df::render_callback
{
    inline void defaultFont( const cShader* _shader, const cFont* _font )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        const cCamera* camera = cCameraManager::getInstance()->current;

        _shader->use();

        _shader->setUniformMatrix4F( "u_projection_view_matrix", camera->projection_view );

        _shader->setUniform4F( "u_color", _font->getLatestColor() );

        _shader->setUniformSampler( "u_texture", 0 );
        _font->bindTexture();

        glEnable( GL_DEPTH_TEST );
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glBindVertexArray( _font->vertex_array_object );

        float x = _font->getLatestPosition().x;
        for( const char& c : _font->getLatestText() )
        {
            const cFont::sCharacter ch = _font->getCharacters().find( c )->second;

            const float xpos = x + ch.bearing.x * _font->getLatestScale().x;
            const float ypos = _font->getLatestPosition().y - ( ch.size.y - ch.bearing.y ) * _font->getLatestScale().y;
            const float zpos = _font->getLatestPosition().z;

            const float w = ch.size.x * _font->getLatestScale().x;
            const float h = ch.size.y * _font->getLatestScale().y;

            const float vertices[ 6 ][ 5 ] = {
                { xpos, ypos + h, zpos, 0.0f, 0.0f },
                { xpos, ypos, zpos, 0.0f, ch.scale.y },
                { xpos + w, ypos, zpos, ch.scale.x, ch.scale.y },

                { xpos, ypos + h, zpos, 0.0f, 0.0f },
                { xpos + w, ypos, zpos, ch.scale.x, ch.scale.y },
                { xpos + w, ypos + h, zpos, ch.scale.x, 0.0f }
            };

            _shader->setUniform1I( "u_layer", c - 32 );

            glBindBuffer( GL_ARRAY_BUFFER, _font->vertex_buffer_object );
            glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( vertices ), vertices );
            glBindBuffer( GL_ARRAY_BUFFER, 0 );

            glDrawArrays( GL_TRIANGLES, 0, 6 );

            x += ( ch.advance >> 6 ) * _font->getLatestScale().x;
        }

        glDisable( GL_DEPTH_TEST );
        glDisable( GL_BLEND );
    }
}
