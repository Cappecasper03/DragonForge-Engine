#include "cQuadManager.h"

#include "engine/managers/cRenderCallbackManager.h"
#include "engine/rendering/OpenGL/callbacks/DefaultQuadCB.h"
#include "engine/rendering/vulkan/assets/cQuad.h"
#include "engine/rendering/Vulkan/callbacks/DefaultQuadCB.h"
#include "engine/rendering/Vulkan/cRenderer.h"

namespace df
{
	cQuadManager::cQuadManager()
	{
		ZoneScoped;

		switch( cRendererSingleton::getInstanceType() )
		{
			case cRendererSingleton::kOpenGL:
				m_default_render_callback = cRenderCallbackManager::create( "default_quad", opengl::render_callback::defaultQuad );
				break;
			case cRendererSingleton::kVulkan:
				createVulkanDefault();
				break;
		}
	}

	iQuad* cQuadManager::create( const std::string& _name, const glm::vec3& _position, const glm::vec2& _size, const cColor& _color )
	{
		switch( cRendererSingleton::getInstanceType() )
		{
			case cRendererSingleton::kOpenGL:
				return iAssetManager::create< opengl::cQuad >( _name, _position, _size, _color );
			case cRendererSingleton::kVulkan:
				return iAssetManager::create< vulkan::cQuad >( _name, _position, _size, _color );
		}

		return nullptr;
	}

	void cQuadManager::createVulkanDefault()
	{
		const vulkan::cRenderer* renderer = reinterpret_cast< vulkan::cRenderer* >( cRendererSingleton::getRenderInstance() );

		vulkan::cPipeline::sCreateInfo pipeline_create_info{
			.logical_device = renderer->logical_device,
			.render_pass    = renderer->render_pass,
		};

		constexpr VkVertexInputBindingDescription binding_description{
			.binding   = 0,
			.stride    = sizeof( vulkan::cQuad::sVertex ),
			.inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
		};
		pipeline_create_info.vertex_input_binding_descriptions.push_back( binding_description );

		std::vector< VkVertexInputAttributeDescription > attribute_descriptions( 2 );
		attribute_descriptions[ 0 ] = {
			.location = 0,
			.binding  = 0,
			.format   = VK_FORMAT_R32G32B32_SFLOAT,
			.offset   = offsetof( vulkan::cQuad::sVertex, position ),
		};

		attribute_descriptions[ 1 ] = {
			.location = 1,
			.binding  = 0,
			.format   = VK_FORMAT_R32G32_SFLOAT,
			.offset   = offsetof( vulkan::cQuad::sVertex, tex_coord ),
		};
		pipeline_create_info.vertex_input_attribute_descriptions.insert( pipeline_create_info.vertex_input_attribute_descriptions.end(), attribute_descriptions.begin(), attribute_descriptions.end() );

		std::vector< VkPipelineShaderStageCreateInfo > shader_stages_create_infos( 2 );
		shader_stages_create_infos[ 0 ] = {
			.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage  = VK_SHADER_STAGE_VERTEX_BIT,
			.module = vulkan::cRenderer::createShaderModule( "default_quad_vertex", renderer->logical_device ),
			.pName  = "main",
		};

		shader_stages_create_infos[ 1 ] = {
			.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage  = VK_SHADER_STAGE_FRAGMENT_BIT,
			.module = vulkan::cRenderer::createShaderModule( "default_quad_fragment", renderer->logical_device ),
			.pName  = "main",
		};
		pipeline_create_info.shader_stages_create_info.insert( pipeline_create_info.shader_stages_create_info.end(), shader_stages_create_infos.begin(), shader_stages_create_infos.end() );

		m_default_render_callback = cRenderCallbackManager::create( "default_quad", pipeline_create_info, vulkan::render_callback::defaultQuad );
	}
}
