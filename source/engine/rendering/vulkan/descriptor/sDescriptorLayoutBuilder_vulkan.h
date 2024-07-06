#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>

#include "sDescriptorAllocator_vulkan.h"

namespace df::vulkan
{
	struct sDescriptorLayoutBuilder_vulkan
	{
		std::vector< vk::DescriptorSetLayoutBinding > bindings;

		void                    addBinding( uint32_t _binding, vk::DescriptorType _type ) { bindings.emplace_back( _binding, _type, 1 ); }
		void                    clear() { bindings.clear(); }
		vk::DescriptorSetLayout build( vk::ShaderStageFlags _shader_stages );
		vk::DescriptorSetLayout build( const vk::UniqueDevice& _logical_device, vk::ShaderStageFlags _shader_stages );
	};
}
