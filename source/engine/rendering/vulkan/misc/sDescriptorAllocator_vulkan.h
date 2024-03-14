#pragma once
#include <deque>
#include <span>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "engine/misc/Misc.h"

namespace df::vulkan
{

	struct sDesctriptorAllocator_vulkan
	{
		DF_DISABLE_COPY_AND_MOVE( sDesctriptorAllocator_vulkan )

		struct sPoolSizeRatio
		{
			VkDescriptorType type;
			float            ratio;
		};

		void create( VkDevice _logical_device, uint32_t _initial_sets, const std::span< sPoolSizeRatio >& _pool_ratios );
		void destroy();
		void clear();

		VkDescriptorSet allocate( VkDescriptorSetLayout _layout );

	protected:
		VkDescriptorPool getPool();
		VkDescriptorPool createPool( uint32_t _set_count, const std::span< sPoolSizeRatio >& _pool_ratios ) const;

		std::vector< sPoolSizeRatio >   m_ratios;
		std::vector< VkDescriptorPool > m_full_pools;
		std::vector< VkDescriptorPool > m_ready_pools;
		uint32_t                        m_sets_per_pool;

	private:
		VkDevice m_logical_device;
	};

	struct sDescriptorWriter_vulkan
	{
		void writeImage( uint32_t _binding, VkImageView _image, VkSampler _sampler, VkImageLayout _layout, VkDescriptorType _type );
		void writeBuffer( uint32_t _binding, VkBuffer _buffer, size_t _size, size_t _offset, VkDescriptorType _type );

		void clear();
		void updateSet( VkDevice _logical_device, VkDescriptorSet _set );

		std::deque< VkDescriptorImageInfo >  image_infos;
		std::deque< VkDescriptorBufferInfo > buffer_infos;
		std::vector< VkWriteDescriptorSet >  writes;
	};
}
