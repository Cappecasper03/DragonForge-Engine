#include "cDescriptorLayoutBuilder_vulkan.h"

#include "engine/graphics/cRenderer.h"
#include "engine/graphics/vulkan/cGraphicsDevice_vulkan.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::vulkan
{

	vk::UniqueDescriptorSetLayout cDescriptorLayoutBuilder_vulkan::build( const vk::ShaderStageFlags _shader_stages )
	{
		DF_ProfilingScopeCpu;

		const cGraphicsDevice_vulkan* renderer = reinterpret_cast< cGraphicsDevice_vulkan* >( cRenderer::getGraphicsDevice() );
		return build( renderer->getLogicalDevice(), _shader_stages );
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
