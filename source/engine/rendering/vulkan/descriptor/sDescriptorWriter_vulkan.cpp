﻿#include "sDescriptorWriter_vulkan.h"

#include <tracy/Tracy.hpp>

namespace df::vulkan
{
	void sDescriptorWriter_vulkan::writeImage( const uint32_t _binding, const VkImageView _image, const VkSampler _sampler, const VkImageLayout _layout, const VkDescriptorType _type )
	{
		ZoneScoped;

		VkDescriptorImageInfo info{
			.sampler     = _sampler,
			.imageView   = _image,
			.imageLayout = _layout,
		};

		const VkWriteDescriptorSet write{
			.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet          = nullptr,
			.dstBinding      = _binding,
			.descriptorCount = 1,
			.descriptorType  = _type,
			.pImageInfo      = &info,
		};

		writes.push_back( write );
	}

	void sDescriptorWriter_vulkan::writeBuffer( const uint32_t _binding, const VkBuffer _buffer, const size_t _size, const size_t _offset, const VkDescriptorType _type )
	{
		ZoneScoped;

		VkDescriptorBufferInfo& info = buffer_infos.emplace_back( VkDescriptorBufferInfo{
			.buffer = _buffer,
			.offset = _offset,
			.range  = _size,
		} );

		const VkWriteDescriptorSet write{
			.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet          = nullptr,
			.dstBinding      = _binding,
			.descriptorCount = 1,
			.descriptorType  = _type,
			.pBufferInfo     = &info,
		};

		writes.push_back( write );
	}
	void sDescriptorWriter_vulkan::clear()
	{
		ZoneScoped;

		image_infos.clear();
		writes.clear();
		buffer_infos.clear();
	}

	void sDescriptorWriter_vulkan::updateSet( const VkDevice _logical_device, const VkDescriptorSet _set )
	{
		for( VkWriteDescriptorSet& write: writes )
			write.dstSet = _set;

		vkUpdateDescriptorSets( _logical_device, static_cast< uint32_t >( writes.size() ), writes.data(), 0, nullptr );
	}
}