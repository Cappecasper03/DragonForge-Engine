#pragma once

#include <vulkan/vulkan.hpp>

#include "engine/core/utils/Misc.h"
#include "types/sAllocatedBuffer_vulkan.h"

namespace df::vulkan
{
	class cGraphicsApi_vulkan;
	class cPipeline_vulkan;

	class cCommandBuffer
	{
	public:
		DF_DeleteCopyAndMove( cCommandBuffer );

		cCommandBuffer()  = default;
		~cCommandBuffer() = default;

		void create( const vk::CommandPool& _command_pool );
		void create( const vk::CommandPool& _command_pool, const cGraphicsApi_vulkan* _graphics_api );

		void destroy();

		void begin( vk::CommandBufferUsageFlagBits _usage = vk::CommandBufferUsageFlagBits::eOneTimeSubmit ) const;
		void end() const;

		void beginRendering( vk::Extent2D                       _extent,
		                     const vk::RenderingAttachmentInfo* _color_attachment = nullptr,
		                     const vk::RenderingAttachmentInfo* _depth_attachment = nullptr ) const;
		void beginRendering( vk::Extent2D                                      _extent,
		                     const std::vector< vk::RenderingAttachmentInfo >& _color_attachments,
		                     const vk::RenderingAttachmentInfo*                _depth_attachment ) const;

		void endRendering() const;

		void bindPipeline( vk::PipelineBindPoint _bind_point, const cPipeline_vulkan* _pipeline ) const;

		void bindDescriptorSets( vk::PipelineBindPoint    _bind_point,
		                         const cPipeline_vulkan*  _pipeline,
		                         unsigned                 _first_set,
		                         unsigned                 _descriptor_set_count,
		                         const vk::DescriptorSet* _descriptor_sets,
		                         unsigned                 _dynamic_offset_count,
		                         const unsigned*          _dynamic_offsets ) const;
		void bindDescriptorSets( vk::PipelineBindPoint                   _bind_point,
		                         const cPipeline_vulkan*                 _pipeline,
		                         unsigned                                _first_set,
		                         const std::vector< vk::DescriptorSet >& _descriptor_sets,
		                         const std::vector< unsigned >&          _dynamic_offsets = {} ) const;

		void pushConstants( const cPipeline_vulkan* _pipeline, vk::ShaderStageFlags _stage_flags, unsigned _offset, unsigned _size, const void* _values ) const;

		void bindVertexBuffers( unsigned _first_binding, unsigned _binding_count, const sAllocatedBuffer_vulkan& _buffer, vk::DeviceSize _offset ) const;
		void bindIndexBuffer( const sAllocatedBuffer_vulkan& _buffer, vk::DeviceSize _offset, vk::IndexType _index_type ) const;

		void drawIndexed( unsigned _index_count, unsigned _instance_count, unsigned _first_index, int _vertex_offset, unsigned _first_instance ) const;

		void setViewport( unsigned _first_viewport, unsigned _viewport_count, const vk::Viewport& _viewport );
		void setScissor( unsigned _first_scissor, unsigned _scissor_count, const vk::Rect2D& _scissor );

		void clearColorImage( vk::Image                        _image,
		                      vk::ImageLayout                  _layout,
		                      const vk::ClearColorValue*       _colors,
		                      uint32_t                         _range_count,
		                      const vk::ImageSubresourceRange* _ranges ) const;

		void transitionImage( const vk::Image& _image, vk::ImageLayout _current_layout, vk::ImageLayout _new_layout ) const;
		void copyImageToImage( const vk::Image& _source, const vk::Image& _destination, vk::Extent2D _source_size, vk::Extent2D _destination_size ) const;

		void copyBuffer( vk::Buffer _source, vk::Buffer _destination, uint32_t _region_count, const vk::BufferCopy* _regions ) const;
		void copyBufferToImage( vk::Buffer _buffer, vk::Image _image, vk::ImageLayout _layout, uint32_t _region_count, const vk::BufferImageCopy* _regions ) const;

		const vk::CommandBuffer& get() const { return m_command_buffer.get(); }

	private:
		vk::UniqueCommandBuffer m_command_buffer;
	};
}
