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
	vk::DescriptorSetLayout cQuad_vulkan::texture_layout = nullptr;

	cQuad_vulkan::cQuad_vulkan( std::string _name, const glm::vec3& _position, const glm::vec2& _size, const cColor& _color )
		: iQuad( std::move( _name ), _position, _size, _color )
	{
		ZoneScoped;

		texture = new cTexture_vulkan( fmt::format( "{}_{}", name, "texture" ) );

		const cRenderer_vulkan* renderer = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );

		const size_t vertex_buffer_size = sizeof( *m_vertices.data() ) * m_vertices.size();
		const size_t index_buffer_size  = sizeof( *m_indices.data() ) * m_indices.size();

		vertex_buffer = helper::util::createBuffer( vertex_buffer_size, vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst, VMA_MEMORY_USAGE_GPU_ONLY );
		index_buffer  = helper::util::createBuffer( index_buffer_size, vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst, VMA_MEMORY_USAGE_GPU_ONLY );

		sAllocatedBuffer staging_buffer = helper::util::createBuffer( vertex_buffer_size + index_buffer_size, vk::BufferUsageFlagBits::eTransferSrc, VMA_MEMORY_USAGE_CPU_ONLY );

		void* data;
		vmaMapMemory( renderer->memory_allocator, staging_buffer.allocation, &data );
		std::memcpy( data, m_vertices.data(), vertex_buffer_size );
		std::memcpy( static_cast< char* >( data ) + vertex_buffer_size, m_indices.data(), index_buffer_size );
		vmaUnmapMemory( renderer->memory_allocator, staging_buffer.allocation );

		renderer->immediateSubmit(
			[ & ]( const vk::CommandBuffer _buffer )
			{
				const vk::BufferCopy vertex_copy( 0, 0, vertex_buffer_size );
				_buffer.copyBuffer( staging_buffer.buffer.get(), vertex_buffer.buffer.get(), 1, &vertex_copy );

				const vk::BufferCopy index_copy( vertex_buffer_size, 0, index_buffer_size );
				_buffer.copyBuffer( staging_buffer.buffer.get(), index_buffer.buffer.get(), 1, &index_copy );
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
