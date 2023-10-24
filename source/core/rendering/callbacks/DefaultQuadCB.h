#pragma once

#include <glad/glad.h>

#include "core/managers/cCameraManager.h"
#include "core/rendering/cShader.h"
#include "core/rendering/assets/cQuad.h"

namespace df::render_callback
{
    static void defaultQuad( const cShader* _shader, const cQuad* _quad )
    {
        const cCamera* camera = cCameraManager::getInstance()->current;

        _shader->use();

        _shader->setUniformMatrix4Fv( "u_model", _quad->transform.world );
        _shader->setUniformMatrix4Fv( "u_projection_view", camera->projection_view );

        _shader->setUniform1B( "u_use_texture", _quad->texture );
        _shader->setUniform4F( "u_color", _quad->color );

        _shader->setUniformSampler( "u_texture", 0 );
        _quad->bindTexture( 0 );

        glBindVertexArray( _quad->vertex_array_object );
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr );
    }
}
