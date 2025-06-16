#include "cCommandBuffer.h"

#include "cGraphicsApi_vulkan.h"
#include "engine/graphics/cRenderer.h"
#include "engine/profiling/ProfilingMacros.h"
#include "pipeline/cPipeline_vulkan.h"
#include "types/Helper_vulkan.h"

namespace df::vulkan
{
	void cCommandBuffer::create( const vk::CommandPool& _command_pool )
	{
		DF_ProfilingScopeCpu;

		create( _command_pool, reinterpret_cast< cGraphicsApi_vulkan* >( cRenderer::getApi() ) );
	}

	void cCommandBuffer::create( const vk::CommandPool& _command_pool, const cGraphicsApi_vulkan* _graphics_api )
	{
		DF_ProfilingScopeCpu;

		const vk::Device&                   logical_device = _graphics_api->getLogicalDevice();
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

		m_command_buffer
			->bindDescriptorSets( _bind_point, _pipeline->m_layout.get(), _first_set, _descriptor_set_count, _descriptor_sets, _dynamic_offset_count, _dynamic_offsets );
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
	void cCommandBuffer::clearColorImage( const vk::Image                  _image,
	                                      const vk::ImageLayout            _layout,
	                                      const vk::ClearColorValue*       _colors,
	                                      const uint32_t                   _range_count,
	                                      const vk::ImageSubresourceRange* _ranges ) const
	{
		DF_ProfilingScopeCpu;

		m_command_buffer->clearColorImage( _image, _layout, _colors, _range_count, _ranges );
	}

	void cCommandBuffer::transitionImage( const vk::Image& _image, const vk::ImageLayout _current_layout, const vk::ImageLayout _new_layout ) const
	{
		DF_ProfilingScopeCpu;

		const vk::ImageSubresourceRange subresource_range( _new_layout == vk::ImageLayout::eDepthAttachmentOptimal ? vk::ImageAspectFlagBits::eDepth
		                                                                                                           : vk::ImageAspectFlagBits::eColor,
		                                                   0,
		                                                   vk::RemainingMipLevels,
		                                                   0,
		                                                   vk::RemainingArrayLayers );

		const vk::ImageMemoryBarrier2 memory_barrier( vk::PipelineStageFlagBits2::eAllCommands,
		                                              vk::AccessFlagBits2::eMemoryWrite,
		                                              vk::PipelineStageFlagBits2::eAllCommands,
		                                              vk::AccessFlagBits2::eMemoryWrite | vk::AccessFlagBits2::eMemoryRead,
		                                              _current_layout,
		                                              _new_layout,
		                                              0,
		                                              0,
		                                              _image,
		                                              subresource_range );

		const vk::DependencyInfo info( vk::DependencyFlags(), 0, nullptr, 0, nullptr, 1, &memory_barrier );
		m_command_buffer->pipelineBarrier2( info );
	}

	void cCommandBuffer::copyImageToImage( const vk::Image& _source, const vk::Image& _destination, const vk::Extent2D _source_size, const vk::Extent2D _destination_size ) const
	{
		DF_ProfilingScopeCpu;

		const std::array< vk::Offset3D, 2 > source{
			{
             vk::Offset3D(),
             vk::Offset3D( static_cast< int32_t >( _source_size.width ), static_cast< int32_t >( _source_size.height ), 1 ),
			 }
		};
		const std::array< vk::Offset3D, 2 > destination{
			{
             vk::Offset3D(),
             vk::Offset3D( static_cast< int32_t >( _destination_size.width ), static_cast< int32_t >( _destination_size.height ), 1 ),
			 }
		};

		const vk::ImageBlit2 blit_region( vk::ImageSubresourceLayers( vk::ImageAspectFlagBits::eColor, 0, 0, 1 ),
		                                  source,
		                                  vk::ImageSubresourceLayers( vk::ImageAspectFlagBits::eColor, 0, 0, 1 ),
		                                  destination );

		const vk::BlitImageInfo2
			blit_info( _source, vk::ImageLayout::eTransferSrcOptimal, _destination, vk::ImageLayout::eTransferDstOptimal, 1, &blit_region, vk::Filter::eLinear );
		m_command_buffer->blitImage2( blit_info );
	}

	void cCommandBuffer::copyBuffer( const vk::Buffer _source, const vk::Buffer _destination, const uint32_t _region_count, const vk::BufferCopy* _regions ) const
	{
		DF_ProfilingScopeCpu;

		m_command_buffer->copyBuffer( _source, _destination, _region_count, _regions );
	}

	void cCommandBuffer::copyBufferToImage( const vk::Buffer           _buffer,
	                                        const vk::Image            _image,
	                                        const vk::ImageLayout      _layout,
	                                        const uint32_t             _region_count,
	                                        const vk::BufferImageCopy* _regions ) const
	{
		DF_ProfilingScopeCpu;

		m_command_buffer->copyBufferToImage( _buffer, _image, _layout, _region_count, _regions );
	}
}