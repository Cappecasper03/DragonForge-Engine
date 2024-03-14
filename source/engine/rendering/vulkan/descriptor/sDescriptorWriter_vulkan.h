#pragma once

#include <deque>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace df::vulkan
{

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
