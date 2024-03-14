#pragma once

/*#include <glad/glad.h>

#include "engine/managers/assets/cCameraManager.h"
#include "engine/rendering/assets/cMesh.h"
#include "engine/rendering/assets/cTexture.h"
#include "engine/rendering/OpenGL/cShader.h"

namespace df::opengl::render_callback
{
    inline void defaultMeshAmbient( const cShader* _shader, const cMesh* _mesh )
    {
        ZoneScoped;

        const cCamera* camera = cCameraManager::getInstance()->current;

        _shader->use();

        _shader->setUniformMatrix4F( "u_world_matrix", _mesh->transform->world );
        _shader->setUniformMatrix4F( "u_view_projection_matrix", camera->view_projection );

        _shader->setUniformSampler( "u_color_texture", 0 );
        _mesh->textures.at( "color" )->bind();

        glEnable( GL_DEPTH_TEST );
        glBindVertexArray( reinterpret_cast< sRendererSpecific* >( _mesh->render_specific )->vertex_array );

        glDrawElements( GL_TRIANGLES, static_cast< GLsizei >( _mesh->indices.size() ), GL_UNSIGNED_INT, nullptr );

        glDisable( GL_DEPTH_TEST );
    }

    inline void defaultMesh( const cShader* _shader, const cMesh* _mesh )
    {
        ZoneScoped;

        const std::string_view name( _shader->name );

        if( name.find( "ambient" ) != std::string::npos )
            defaultMeshAmbient( _shader, _mesh );
    }

    inline void defaultMeshDeferred( const cShader* _shader, const cMesh* _mesh )
    {
        ZoneScoped;

        const cCamera* camera = cCameraManager::getInstance()->current;

        _shader->use();

        _shader->setUniformMatrix4F( "u_world_matrix", _mesh->transform->world );
        _shader->setUniformMatrix4F( "u_view_projection_matrix", camera->view_projection );

        _shader->setUniformSampler( "u_color_texture", 0 );
        _mesh->textures.at( "color" )->bind( 0 );

        _shader->setUniformSampler( "u_normal_texture", 1 );
        _mesh->textures.at( "normal" )->bind( 1 );

        _shader->setUniformSampler( "u_specular_texture", 2 );
        _mesh->textures.at( "specular" )->bind( 2 );

        glEnable( GL_DEPTH_TEST );
        glBindVertexArray( reinterpret_cast< sRendererSpecific* >( _mesh->render_specific )->vertex_array );

        glDrawElements( GL_TRIANGLES, static_cast< GLsizei >( _mesh->indices.size() ), GL_UNSIGNED_INT, nullptr );

        glDisable( GL_DEPTH_TEST );
    }
}*/
