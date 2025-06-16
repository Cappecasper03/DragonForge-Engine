#include "cDescriptorLayoutBuilder_vulkan.h"

#include "engine/graphics/cRenderer.h"
#include "engine/graphics/vulkan/cGraphicsApi_vulkan.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::vulkan
{

	vk::UniqueDescriptorSetLayout cDescriptorLayoutBuilder_vulkan::build( const vk::ShaderStageFlags _shader_stages )
	{
		DF_ProfilingScopeCpu;

		const cGraphicsApi_vulkan* graphics_api = reinterpret_cast< cGraphicsApi_vulkan* >( cRenderer::getApi() );
		return build( graphics_api->getLogicalDevice(), _shader_stages );
	}

	vk::UniqueDescriptorSetLayout cDescriptorLayoutBuilder_vulkan::build( const vk::Device& _logical_device, const vk::ShaderStageFlags _shader_stages )
	{
		DF_ProfilingScopeCpu;

		for( vk::DescriptorSetLayoutBinding& binding: bindings )
			binding.stageFlags |= _shader_stages;

		const vk::DescriptorSetLayoutCreateInfo create_info( vk::DescriptorSetLayoutCreateFlags(), static_cast< uint32_t >( bindings.size() ), bindings.data() );

		return _logical_device.createDescriptorSetLayoutUnique( create_info ).value;
	}
}
