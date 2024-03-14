#include "sDescriptorLayoutBuilder_vulkan.h"

#include <tracy/Tracy.hpp>

namespace df::vulkan
{
	void sDescriptorLayoutBuilder_vulkan::addBinding( const uint32_t _binding, const VkDescriptorType _type )
	{
		ZoneScoped;

		const VkDescriptorSetLayoutBinding binding{
			.binding         = _binding,
			.descriptorType  = _type,
			.descriptorCount = 1,
		};

		bindings.push_back( binding );
	}

	void sDescriptorLayoutBuilder_vulkan::clear()
	{
		ZoneScoped;

		bindings.clear();
	}

	VkDescriptorSetLayout sDescriptorLayoutBuilder_vulkan::build( VkDevice _logical_device, VkShaderStageFlags _shader_stages )
	{
		ZoneScoped;

		for( VkDescriptorSetLayoutBinding& binding: bindings )
			binding.stageFlags |= _shader_stages;

		const VkDescriptorSetLayoutCreateInfo create_info{
			.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.pNext        = nullptr,
			.flags        = 0,
			.bindingCount = static_cast< uint32_t >( bindings.size() ),
			.pBindings    = bindings.data(),
		};

		VkDescriptorSetLayout layout;
		vkCreateDescriptorSetLayout( _logical_device, &create_info, nullptr, &layout );
		return layout;
	}
}
