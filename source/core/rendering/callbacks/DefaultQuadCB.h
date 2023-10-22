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

        _shader->setUniformMatrix4Fv( "u_model", _quad->matrix );
        _shader->setUniformMatrix4Fv( "u_view", camera->view );
        _shader->setUniformMatrix4Fv( "u_projection", camera->projection );

        _shader->setUniform4F( "u_color", _quad->color );

        glBindVertexArray( _quad->vertex_array_object );
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
    }
}
