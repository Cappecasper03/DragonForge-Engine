#pragma once

#include <glad/glad.h>

#include "core/managers/cCameraManager.h"
#include "core/rendering/cShader.h"
#include "core/rendering/assets/cFont.h"

namespace df::render_callback
{
    inline void defaultFont( const cShader* _shader, const cFont* _font )
    {
        const cCamera* camera = cCameraManager::getInstance()->current;

        _shader->use();

        _shader->setUniformMatrix4F( "u_projection_view", camera->projection_view );

        _shader->setUniform4F( "u_color", _font->color );

        _shader->setUniformSampler( "u_texture", 0 );
        _font->bindTexture();

        glBindVertexArray( _font->vertex_array_object );

        float x = _font->position.x;
        for( const char& c : _font->text )
        {
            const cFont::sCharacter ch = _font->characters.find( c )->second;

            const float xpos = x + ch.bearing.x * _font->scale.x;
            const float ypos = _font->position.y - ( ch.size.y - ch.bearing.y ) * _font->scale.y;

            const float w = ch.size.x * _font->scale.x;
            const float h = ch.size.y * _font->scale.y;

            const float vertices[ 6 ][ 4 ] = {
                { xpos, ypos + h, 0.0f, 0.0f },
                { xpos, ypos, 0.0f, ch.scale.y },
                { xpos + w, ypos, ch.scale.x, ch.scale.y },

                { xpos, ypos + h, 0.0f, 0.0f },
                { xpos + w, ypos, ch.scale.x, ch.scale.y },
                { xpos + w, ypos + h, ch.scale.x, 0.0f }
            };

            _shader->setUniform1I( "u_layer", c - 32 );

            glBindBuffer( GL_ARRAY_BUFFER, _font->vertex_buffer_object );
            glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( vertices ), vertices );
            glBindBuffer( GL_ARRAY_BUFFER, 0 );

            glDrawArrays( GL_TRIANGLES, 0, 6 );

            x += ( ch.advance >> 6 ) * _font->scale.x;
        }

        glBindVertexArray( 0 );
    }
}
