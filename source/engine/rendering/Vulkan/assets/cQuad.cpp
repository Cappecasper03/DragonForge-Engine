#include "cQuad.h"

#include "engine/managers/assets/cQuadManager.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/rendering/cRendererSingleton.h"
#include "engine/rendering/Vulkan/cRenderer.h"

namespace df::vulkan
{
	cQuad::cQuad( std::string _name, const glm::vec3& _position, const glm::vec2& _size, const cColor& _color )
		: iQuad( std::move( _name ), _position, _size, _color )
	{
		ZoneScoped;

		const cRenderer* renderer = reinterpret_cast< cRenderer* >( cRendererSingleton::getRenderInstance() );

		constexpr VkBufferCreateInfo buffer_create_info{
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

		const VkMemoryAllocateInfo memory_allocate_info{
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
