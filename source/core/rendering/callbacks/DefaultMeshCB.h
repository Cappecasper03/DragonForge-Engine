#pragma once

#include <glad/glad.h>

#include "core/managers/cCameraManager.h"
#include "core/rendering/cShader.h"
#include "core/rendering/assets/cMesh.h"
#include "core/rendering/assets/cTexture.h"

namespace df::render_callback
{
    inline void defaultMeshAmbient( const cShader* _shader, const cMesh* _mesh )
    {
        const cCamera* camera = cCameraManager::getInstance()->current;

        _shader->use();

        _shader->setUniformMatrix4F( "u_model", _mesh->transform.world );
        _shader->setUniformMatrix4F( "u_projection_view", camera->projection_view );

        _shader->setUniform4F( "u_color", _mesh->color );

        _shader->setUniformSampler( "u_color_texture", 0 );
        _mesh->textures.at( "color" )->bind();

        glEnable( GL_DEPTH_TEST );
        glBindVertexArray( _mesh->vertex_array );

        glDrawElements( GL_TRIANGLES, static_cast< GLsizei >( _mesh->indices.size() ), GL_UNSIGNED_INT, nullptr );

        glDisable( GL_DEPTH_TEST );
    }

    inline void defaultMesh( const cShader* _shader, const cMesh* _mesh )
    {
        const std::string_view name( _shader->name );

        if( name.find( "ambient" ) != std::string::npos )
            defaultMeshAmbient( _shader, _mesh );
    }
}
