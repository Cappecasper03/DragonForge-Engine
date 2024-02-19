﻿#include "cQuad.h"

#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <vulkan/vulkan_core.h>

#include "engine/managers/cRenderCallbackManager.h"
#include "engine/managers/assets/cQuadManager.h"
#include "engine/rendering/cRendererSingleton.h"
#include "engine/rendering/assets/cTexture.h"
#include "engine/rendering/OpenGL/callbacks/DefaultFontCB.h"
#include "engine/rendering/Vulkan/cRenderer.h"

namespace df
{
    bool cQuad::m_initialized_once = false;

    cQuad::cQuad( std::string _name, const glm::vec3& _position, const glm::vec2& _size, const cColor& _color, const std::string& _texture_file )
    : iRenderAsset( std::move( _name ) ),
      texture( nullptr ),
      color( _color ),
      m_vertices{},
      m_indices{ 0, 1, 3, 1, 2, 3 }
    {
        ZoneScoped;

        transform.local = translate( transform.world, _position );
        transform.update();

        m_vertices[ 0 ] = { glm::vec3( _size.x / 2, _size.y / 2, 0 ), glm::vec2( 1, 1 ) };
        m_vertices[ 1 ] = { glm::vec3( _size.x / 2, -_size.y / 2, 0 ), glm::vec2( 1, 0 ) };
        m_vertices[ 2 ] = { glm::vec3( -_size.x / 2, -_size.y / 2, 0 ), glm::vec2( 0, 0 ) };
        m_vertices[ 3 ] = { glm::vec3( -_size.x / 2, _size.y / 2, 0 ), glm::vec2( 0, 1 ) };

        switch( cRendererSingleton::getRenderInstanceType() )
        {
            case cRendererSingleton::kOpenGL: { initOpenGL( _texture_file ); }
            break;
            case cRendererSingleton::kVulkan: { initVulkan( _texture_file ); }
            break;
        }

        if( !m_initialized_once )
            m_initialized_once = true;
    }

    cQuad::~cQuad()
    {
        ZoneScoped;

        delete texture;
    }

    void cQuad::render()
    {
        ZoneScoped;

        switch( cRendererSingleton::getRenderInstanceType() )
        {
            case cRendererSingleton::kOpenGL:
            {
                if( cQuadManager::getForcedRenderCallback() )
                    cRenderCallbackManager::render< opengl::cShader >( cQuadManager::getForcedRenderCallback(), this );
                else if( render_callback )
                    cRenderCallbackManager::render< opengl::cShader >( render_callback, this );
                else
                    cRenderCallbackManager::render< opengl::cShader >( cQuadManager::getDefaultRenderCallback(), this );
            }
            break;
            case cRendererSingleton::kVulkan:
            {}
            break;
        }
    }

    void cQuad::bindTexture( const int& _index ) const
    {
        ZoneScoped;

        if( texture )
            texture->bind( _index );
    }

    void cQuad::initOpenGL( const std::string& _texture_file )
    {
        ZoneScoped;

        const opengl::sRendererSpecific* specific = reinterpret_cast< opengl::sRendererSpecific* >( render_specific );

        glBindVertexArray( specific->vertex_array );

        glBindBuffer( GL_ARRAY_BUFFER, specific->vertex_buffer );
        glBufferData( GL_ARRAY_BUFFER, sizeof( m_vertices ), m_vertices, GL_STATIC_DRAW );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, specific->element_buffer );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( m_indices ), m_indices, GL_STATIC_DRAW );

        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( *m_vertices ), nullptr );
        glEnableVertexAttribArray( 0 );

        glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( *m_vertices ), reinterpret_cast< void* >( sizeof( m_vertices->position ) ) );
        glEnableVertexAttribArray( 1 );

        glBindVertexArray( 0 );

        if( !_texture_file.empty() )
        {
            texture = new cTexture( std::format( "{}_quad", name ), GL_TEXTURE_2D );
            texture->load( _texture_file );
            texture->setTextureParameterI( GL_TEXTURE_WRAP_S, GL_REPEAT );
            texture->setTextureParameterI( GL_TEXTURE_WRAP_T, GL_REPEAT );
            texture->setTextureParameterI( GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
            texture->setTextureParameterI( GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        }
    }

    void cQuad::initVulkan( const std::string& /*_texture_file*/ )
    {
        ZoneScoped;

        if( m_initialized_once )
            return;

        vulkan::cRenderer* renderer = reinterpret_cast< vulkan::cRenderer* >( cRendererSingleton::getRenderInstance() );

        VkVertexInputBindingDescription binding_description{};
        binding_description.binding   = 0;
        binding_description.stride    = sizeof( sVertex );
        binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        renderer->vertex_input_binding_descriptions.push_back( binding_description );

        std::vector< VkVertexInputAttributeDescription > attribute_descriptions;
        VkVertexInputAttributeDescription                attribute_description;

        attribute_description.binding  = 0;
        attribute_description.location = 0;
        attribute_description.format   = VK_FORMAT_R32G32B32_SFLOAT;
        attribute_description.offset   = offsetof( sVertex, position );
        attribute_descriptions.push_back( attribute_description );

        attribute_description.binding  = 0;
        attribute_description.location = 1;
        attribute_description.format   = VK_FORMAT_R32G32_SFLOAT;
        attribute_description.offset   = offsetof( sVertex, tex_coord );
        attribute_descriptions.push_back( attribute_description );

        renderer->vertex_input_attribute_descriptions.insert( renderer->vertex_input_attribute_descriptions.end(), attribute_descriptions.begin(), attribute_descriptions.end() );
        renderer->recreateGraphicsPipeline();
    }
}
