#include "cCommandBuffer.h"

#include "cRenderer_vulkan.h"
#include "engine/graphics/cRenderer.h"
#include "pipeline/cPipeline_vulkan.h"
#include "engine/profiling/ProfilingMacros.h"
#include "types/Helper_vulkan.h"

namespace df::vulkan
{
	void cCommandBuffer::create( const vk::CommandPool& _command_pool )
	{
		DF_ProfilingScopeCpu;

		create( _command_pool, reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() ) );
	}

	void cCommandBuffer::create( const vk::CommandPool& _command_pool, const cRenderer_vulkan* _renderer )
	{
		DF_ProfilingScopeCpu;

		const vk::Device&                   logical_device = _renderer->getLogicalDevice();
		const vk::CommandBufferAllocateInfo allocate_info( _command_pool, vk::CommandBufferLevel::ePrimary, 1 );

		m_command_buffer.swap( logical_device.allocateCommandBuffersUnique( allocate_info ).value.front() );
	}

	void cCommandBuffer::destroy()
	{
		DF_ProfilingScopeCpu;

		m_command_buffer.reset();
	}

	void cCommandBuffer::begin( const vk::CommandBufferUsageFlagBits _usage ) const
	{
		DF_ProfilingScopeCpu;

		const vk::CommandBufferBeginInfo begin_info( _usage );

		if( m_command_buffer->begin( &begin_info ) != vk::Result::eSuccess )
			assert( false );
	}

	void cCommandBuffer::end() const
	{
		DF_ProfilingScopeCpu;

		if( m_command_buffer->end() != vk::Result::eSuccess )
			assert( false );
	}

	void cCommandBuffer::beginRendering( const vk::Extent2D                 _extent,
	                                     const vk::RenderingAttachmentInfo* _color_attachment,
	                                     const vk::RenderingAttachmentInfo* _depth_attachment ) const
	{
		DF_ProfilingScopeCpu;

		const vk::RenderingInfo info( vk::RenderingFlags(), vk::Rect2D( vk::Offset2D(), _extent ), 1, 0, 1, _color_attachment, _depth_attachment );

		m_command_buffer->beginRendering( &info );
	}

	void cCommandBuffer::beginRendering( const vk::Extent2D                                _extent,
	                                     const std::vector< vk::RenderingAttachmentInfo >& _color_attachments,
	                                     const vk::RenderingAttachmentInfo*                _depth_attachment = nullptr ) const
	{
		DF_ProfilingScopeCpu;

		const vk::RenderingInfo info( vk::RenderingFlags(), vk::Rect2D( vk::Offset2D(), _extent ), 1, 0, _color_attachments, _depth_attachment );

		m_command_buffer->beginRendering( &info );
	}

	void cCommandBuffer::endRendering() const
	{
		DF_ProfilingScopeCpu;

		m_command_buffer->endRendering();
	}

	void cCommandBuffer::bindPipeline( const vk::PipelineBindPoint _bind_point, const cPipeline_vulkan* _pipeline ) const
	{
		DF_ProfilingScopeCpu;

		m_command_buffer->bindPipeline( _bind_point, _pipeline->m_pipeline.get() );
	}

	void cCommandBuffer::bindDescriptorSets( const vk::PipelineBindPoint _bind_point,
	                                         const cPipeline_vulkan*     _pipeline,
	                                         const unsigned              _first_set,
	                                         const unsigned              _descriptor_set_count,
	                                         const vk::DescriptorSet*    _descriptor_sets,
	                                         const unsigned              _dynamic_offset_count,
	                                         const unsigned*             _dynamic_offsets ) const
	{
		DF_ProfilingScopeCpu;

		m_command_buffer->bindDescriptorSets( _bind_point, _pipeline->m_layout.get(), _first_set, _descriptor_set_count, _descriptor_sets, _dynamic_offset_count, _dynamic_offsets );
	}

	void cCommandBuffer::bindDescriptorSets( const vk::PipelineBindPoint             _bind_point,
	                                         const cPipeline_vulkan*                 _pipeline,
	                                         const unsigned                          _first_set,
	                                         const std::vector< vk::DescriptorSet >& _descriptor_sets,
	                                         const std::vector< unsigned >&          _dynamic_offsets ) const
	{
		DF_ProfilingScopeCpu;

		m_command_buffer->bindDescriptorSets( _bind_point, _pipeline->m_layout.get(), _first_set, _descriptor_sets, _dynamic_offsets );
	}

	void cCommandBuffer::pushConstants( const cPipeline_vulkan*    _pipeline,
	                                    const vk::ShaderStageFlags _stage_flags,
	                                    const unsigned             _offset,
	                                    const unsigned             _size,
	                                    const void*                _values ) const
	{
		DF_ProfilingScopeCpu;

		m_command_buffer->pushConstants( _pipeline->m_layout.get(), _stage_flags, _offset, _size, _values );
	}

	void cCommandBuffer::bindVertexBuffers( const unsigned                 _first_binding,
	                                        const unsigned                 _binding_count,
	                                        const sAllocatedBuffer_vulkan& _buffer,
	                                        const vk::DeviceSize           _offset ) const
	{
		DF_ProfilingScopeCpu;

		m_command_buffer->bindVertexBuffers( _first_binding, _binding_count, &_buffer.buffer.get(), &_offset );
	}

	void cCommandBuffer::bindIndexBuffer( const sAllocatedBuffer_vulkan& _buffer, const vk::DeviceSize _offset, const vk::IndexType _index_type ) const
	{
		DF_ProfilingScopeCpu;

		m_command_buffer->bindIndexBuffer( _buffer.buffer.get(), _offset, _index_type );
	}

	void cCommandBuffer::drawIndexed( const unsigned _index_count,
	                                  const unsigned _instance_count,
	                                  const unsigned _first_index,
	                                  const int      _vertex_offset,
	                                  const unsigned _first_instance ) const
	{
		DF_ProfilingScopeCpu;

		m_command_buffer->drawIndexed( _index_count, _instance_count, _first_index, _vertex_offset, _first_instance );
	}

	void cCommandBuffer::setViewport( const unsigned _first_viewport, const unsigned _viewport_count, const vk::Viewport& _viewport )
	{
		DF_ProfilingScopeCpu;

		m_command_buffer->setViewport( _first_viewport, _viewport_count, &_viewport );
	}

	void cCommandBuffer::setScissor( const unsigned _first_scissor, const unsigned _scissor_count, const vk::Rect2D& _scissor )
	{
		DF_ProfilingScopeCpu;

		m_command_buffer->setScissor( _first_scissor, _scissor_count, &_scissor );
	}
}