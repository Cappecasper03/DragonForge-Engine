#include "cQuad_vulkan.h"

#include <vk_mem_alloc.h>

#include "cTexture_vulkan.h"
#include "engine/managers/assets/cQuadManager.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/rendering/cRenderer.h"
#include "engine/rendering/vulkan/cRenderer_vulkan.h"
#include "engine/rendering/vulkan/misc/Helper_vulkan.h"

namespace df::vulkan
{
	cQuad_vulkan::cQuad_vulkan( std::string _name, const glm::vec3& _position, const glm::vec2& _size, const cColor& _color )
		: iQuad( std::move( _name ), _position, _size, _color )
	{
		ZoneScoped;

		texture = new cTexture_vulkan( fmt::format( "{}_{}", name, "texture" ) );

		const cRenderer_vulkan* renderer = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );

		const size_t vertex_buffer_size = sizeof( *m_vertices.data() ) * m_vertices.size();

		vertex_buffer = helper::util::createBuffer( vertex_buffer_size,
		                                            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
		                                            VMA_MEMORY_USAGE_GPU_ONLY );

		const size_t index_buffer_size = sizeof( *m_indices.data() ) * m_indices.size();
		index_buffer                   = helper::util::createBuffer( index_buffer_size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY );

		const VkBufferDeviceAddressInfo address_info{
			.sType  = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
			.buffer = vertex_buffer.buffer,
		};

		vertex_buffer_address = vkGetBufferDeviceAddress( renderer->logical_device, &address_info );

		fragment_buffer = helper::util::createBuffer( sizeof( sFragmentUniforms ), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, renderer->memory_allocator );

		sAllocatedBuffer staging_buffer = helper::util::createBuffer( vertex_buffer_size + index_buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY );

		void* data = staging_buffer.allocation_info.pMappedData;

		std::memcpy( data, m_vertices.data(), vertex_buffer_size );
		std::memcpy( static_cast< char* >( data ) + vertex_buffer_size, m_indices.data(), index_buffer_size );

		renderer->immediateSubmit(
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

		helper::util::destroyBuffer( staging_buffer );
	}

	bool cQuad_vulkan::loadTexture( const std::string& _file_path, const bool _mipmapped, const int _mipmaps, const bool _flip_vertically_on_load )
	{
		ZoneScoped;

		return texture->load( _file_path, _mipmapped, _mipmaps, _flip_vertically_on_load );
	}

	void cQuad_vulkan::render()
	{
		ZoneScoped;

		if( cQuadManager::getForcedRenderCallback() )
			cRenderCallbackManager::render< cPipeline_vulkan >( cQuadManager::getForcedRenderCallback(), this );
		else if( render_callback )
			cRenderCallbackManager::render< cPipeline_vulkan >( render_callback, this );
		else
			cRenderCallbackManager::render< cPipeline_vulkan >( cQuadManager::getDefaultRenderCallback(), this );
	}
}
