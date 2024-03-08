#include "cQuad.h"

#include "engine/managers/assets/cQuadManager.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/rendering/cRendererSingleton.h"
#include "engine/rendering/Vulkan/cPipelineManager.h"
#include "engine/rendering/Vulkan/cRenderer.h"

namespace df::vulkan
{
	bool cQuad::m_initialized_once = false;

	cQuad::cQuad( std::string _name, const glm::vec3& _position, const glm::vec2& _size, const cColor& _color )
		: iQuad( std::move( _name ), _position, _size, _color )
	{
		ZoneScoped;

		const cRenderer* renderer = reinterpret_cast< cRenderer* >( cRendererSingleton::getRenderInstance() );

		VkBufferCreateInfo buffer_create_info{
			.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size        = sizeof( sVertex ) * 4,
			.usage       = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		};

		if( vkCreateBuffer( renderer->logical_device, &buffer_create_info, nullptr, &vertex_buffer ) != VK_SUCCESS )
		{
			DF_LOG_ERROR( "Failed to create buffer" );
			return;
		}

		VkMemoryRequirements memory_requirements{};
		vkGetBufferMemoryRequirements( renderer->logical_device, vertex_buffer, &memory_requirements );

		VkMemoryAllocateInfo memory_allocate_info{
			.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.allocationSize  = memory_requirements.size,
			.memoryTypeIndex = cRenderer::findMemoryType( memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, renderer->physical_device ),
		};

		if( vkAllocateMemory( renderer->logical_device, &memory_allocate_info, nullptr, &vertex_buffer_memory ) != VK_SUCCESS )
		{
			DF_LOG_ERROR( "Failed to allocate memory" );
			return;
		}

		vkBindBufferMemory( renderer->logical_device, vertex_buffer, vertex_buffer_memory, 0 );

		if( m_initialized_once )
			return;

		m_initialized_once = true;

		cPipeline::sCreateInfo pipeline_create_info{
			.logical_device = renderer->logical_device,
			.render_pass    = renderer->render_pass,
		};

		VkVertexInputBindingDescription binding_description{
			.binding   = 0,
			.stride    = sizeof( sVertex ),
			.inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
		};
		pipeline_create_info.vertex_input_binding_descriptions.push_back( binding_description );

		std::vector< VkVertexInputAttributeDescription > attribute_descriptions( 2 );
		attribute_descriptions[ 0 ] = {
			.location = 0,
			.binding  = 0,
			.format   = VK_FORMAT_R32G32B32_SFLOAT,
			.offset   = offsetof( sVertex, position ),
		};

		attribute_descriptions[ 1 ] = {
			.location = 1,
			.binding  = 0,
			.format   = VK_FORMAT_R32G32_SFLOAT,
			.offset   = offsetof( sVertex, tex_coord ),
		};
		pipeline_create_info.vertex_input_attribute_descriptions.insert( pipeline_create_info.vertex_input_attribute_descriptions.end(), attribute_descriptions.begin(), attribute_descriptions.end() );

		std::vector< VkPipelineShaderStageCreateInfo > shader_stages_create_infos( 2 );
		shader_stages_create_infos[ 0 ] = {
			.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage  = VK_SHADER_STAGE_VERTEX_BIT,
			.module = cRenderer::createShaderModule( "default_quad_vertex", renderer->logical_device ),
			.pName  = "main",
		};

		shader_stages_create_infos[ 1 ] = {
			.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage  = VK_SHADER_STAGE_FRAGMENT_BIT,
			.module = cRenderer::createShaderModule( "default_quad_fragment", renderer->logical_device ),
			.pName  = "main",
		};
		pipeline_create_info.shader_stages_create_info.insert( pipeline_create_info.shader_stages_create_info.end(), shader_stages_create_infos.begin(), shader_stages_create_infos.end() );

		cPipelineManager::create( "default_quad", pipeline_create_info );
	}

	void cQuad::render()
	{
		ZoneScoped;

		if( cQuadManager::getForcedRenderCallback() )
			cRenderCallbackManager::render< cPipeline >( cQuadManager::getForcedRenderCallback(), this );
		else if( render_callback )
			cRenderCallbackManager::render< cPipeline >( render_callback, this );
		else
			cRenderCallbackManager::render< cPipeline >( cQuadManager::getDefaultRenderCallback(), this );
	}
}
