﻿#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>

namespace df::vulkan
{
	class cDescriptorLayoutBuilder_vulkan
	{
	public:
		std::vector< vk::DescriptorSetLayoutBinding > bindings;

		void                          addBinding( uint32_t _binding, vk::DescriptorType _type ) { bindings.emplace_back( _binding, _type, 1 ); }
		void                          clear() { bindings.clear(); }
		vk::UniqueDescriptorSetLayout build( vk::ShaderStageFlags _shader_stages );
		vk::UniqueDescriptorSetLayout build( const vk::Device& _logical_device, vk::ShaderStageFlags _shader_stages );
	};
}
