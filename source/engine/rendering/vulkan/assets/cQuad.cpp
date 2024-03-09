#include "cQuad.h"

#include <vk_mem_alloc.h>

#include "engine/managers/assets/cQuadManager.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/rendering/cRendererSingleton.h"
#include "engine/rendering/vulkan/cRenderer.h"

namespace df::vulkan
{
	cQuad::cQuad( std::string _name, const glm::vec3& _position, const glm::vec2& _size, const cColor& _color )
		: iQuad( std::move( _name ), _position, _size, _color )
	{
		ZoneScoped;

		const cRenderer* renderer = reinterpret_cast< cRenderer* >( cRendererSingleton::getRenderInstance() );

		const size_t vertex_buffer_size = sizeof( *m_vertices.data() ) * m_vertices.size();
		if( !renderer->createBuffer( vertex_buffer_size,
		                             VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
		                             VMA_MEMORY_USAGE_GPU_ONLY,
		                             vertex_buffer ) )
		{
			DF_LOG_ERROR( "Failed to create vertex buffer" );
			return;
		}

		const size_t index_buffer_size = sizeof( *m_indices.data() ) * m_indices.size();
		if( !renderer->createBuffer( index_buffer_size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY, index_buffer ) )
		{
			DF_LOG_ERROR( "Failed to create index buffer" );
			return;
		}

		const VkBufferDeviceAddressInfo address_info{
			.sType  = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
			.buffer = vertex_buffer.buffer,
		};

		vertex_buffer_address = vkGetBufferDeviceAddress( renderer->logical_device, &address_info );

		sBuffer staging_buffer;
		if( !renderer->createBuffer( vertex_buffer_size + index_buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY, staging_buffer ) )
		{
			DF_LOG_ERROR( "Failed to create staging buffer" );
			return;
		}

		void* data = staging_buffer.allocation_info.pMappedData;

		std::memcpy( data, m_vertices.data(), vertex_buffer_size );
		std::memcpy( static_cast< char* >( data ) + vertex_buffer_size, m_indices.data(), index_buffer_size );

		renderer->submit(
			[ & ]( const VkCommandBuffer _buffer )
			{
				const VkBufferCopy vertex_copy{
					.srcOffset = 0,
					.dstOffset = 0,
					.size      = vertex_buffer_size,
				};

				vkCmdCopyBuffer( _buffer, staging_buffer.buffer, vertex_buffer.buffer, 1, &vertex_copy );

				const VkBufferCopy index_copy{
					.srcOffset = 0,
					.dstOffset = 0,
					.size      = index_buffer_size,
				};

				vkCmdCopyBuffer( _buffer, staging_buffer.buffer, index_buffer.buffer, 1, &index_copy );
			} );

		vmaDestroyBuffer( renderer->memory_allocator, staging_buffer.buffer, staging_buffer.allocation );
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
