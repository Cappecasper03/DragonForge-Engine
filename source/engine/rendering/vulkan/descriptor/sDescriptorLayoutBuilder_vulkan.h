#pragma once
#include <vector>

#include "sDescriptorAllocator_vulkan.h"

namespace df::vulkan
{
	struct sDescriptorLayoutBuilder_vulkan
	{
		std::vector< VkDescriptorSetLayoutBinding > bindings;

		void                  addBinding( uint32_t _binding, VkDescriptorType _type );
		void                  clear();
		VkDescriptorSetLayout build( VkDevice _logical_device, VkShaderStageFlags _shader_stages );
	};
}
