#include "cQuad.h"

#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <vulkan/vulkan_core.h>

#include "engine/managers/cRenderCallbackManager.h"
#include "engine/managers/assets/cQuadManager.h"
#include "engine/rendering/cRendererSingleton.h"
#include "engine/rendering/assets/cTexture.h"
#include "engine/rendering/OpenGL/callbacks/DefaultFontCB.h"
#include "engine/rendering/Vulkan/cPipeline.h"
#include "engine/rendering/Vulkan/cPipelineManager.h"
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

        transform->local = translate( transform->world, _position );
        transform->update();

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

        m_initialized_once                = true;
        const vulkan::cRenderer* renderer = reinterpret_cast< vulkan::cRenderer* >( cRendererSingleton::getRenderInstance() );

        vulkan::cPipeline::sCreateInfo create_info{};
        create_info.logical_device = renderer->logical_device;
        create_info.render_pass    = renderer->render_pass;

        VkVertexInputBindingDescription binding_description{};
        binding_description.binding   = 0;
        binding_description.stride    = sizeof( sVertex );
        binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        create_info.vertex_input_binding_descriptions.push_back( binding_description );

        std::vector< VkVertexInputAttributeDescription > attribute_descriptions( 2 );
        attribute_descriptions[ 0 ].binding  = 0;
        attribute_descriptions[ 0 ].location = 0;
        attribute_descriptions[ 0 ].format   = VK_FORMAT_R32G32B32_SFLOAT;
        attribute_descriptions[ 0 ].offset   = offsetof( sVertex, position );

        attribute_descriptions[ 1 ].binding  = 0;
        attribute_descriptions[ 1 ].location = 1;
        attribute_descriptions[ 1 ].format   = VK_FORMAT_R32G32_SFLOAT;
        attribute_descriptions[ 1 ].offset   = offsetof( sVertex, tex_coord );
        create_info.vertex_input_attribute_descriptions.insert( create_info.vertex_input_attribute_descriptions.end(), attribute_descriptions.begin(), attribute_descriptions.end() );

        std::vector< VkPipelineShaderStageCreateInfo > shader_stages_create_info( 2 );
        shader_stages_create_info[ 0 ].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shader_stages_create_info[ 0 ].stage  = VK_SHADER_STAGE_VERTEX_BIT;
        shader_stages_create_info[ 0 ].module = vulkan::cRenderer::createShaderModule( "default_quad_vertex", renderer->logical_device );
        shader_stages_create_info[ 0 ].pName  = "main";

        shader_stages_create_info[ 1 ].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shader_stages_create_info[ 1 ].stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
        shader_stages_create_info[ 1 ].module = vulkan::cRenderer::createShaderModule( "default_quad_fragment", renderer->logical_device );
        shader_stages_create_info[ 1 ].pName  = "main";
        create_info.shader_stages_create_info.insert( create_info.shader_stages_create_info.end(), shader_stages_create_info.begin(), shader_stages_create_info.end() );

        vulkan::cPipelineManager::create( "default_quad", create_info );
    }
}
