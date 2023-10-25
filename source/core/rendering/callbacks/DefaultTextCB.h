#pragma once

#include <glad/glad.h>

#include "core/managers/cCameraManager.h"
#include "core/rendering/cShader.h"
#include "core/rendering/assets/cFont.h"

namespace df::render_callback
{
    inline void defaultText( const cShader* _shader, const cFont* _font )
    {
        const cCamera* camera = cCameraManager::getInstance()->current;

        _shader->use();

        _shader->setUniformMatrix4Fv( "u_projection_view", camera->projection_view );

        _shader->setUniform4F( "u_color", _font->color );

        _shader->setUniformSampler( "u_texture_array", 0 );
        _font->getBitmap()->bind();

        glBindVertexArray( _font->vertex_array_object );

        for( auto it = _font->latest_text.begin(); it != _font->latest_text.end(); ++it )
        {
            auto character_it = _font->m_characters.find( *it );
            if( character_it == _font->m_characters.end() )
                continue;

            _shader->setUniformMatrix4Fv( "u_model", _font->transform.world );
            _shader->setUniform1F( "u_layer", character_it->first );
            glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr );
        }
    }
}
