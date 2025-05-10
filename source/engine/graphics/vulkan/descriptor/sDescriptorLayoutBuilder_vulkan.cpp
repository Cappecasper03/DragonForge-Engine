#include "sDescriptorLayoutBuilder_vulkan.h"

#include "engine/profiling/ProfilingMacros.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/vulkan/cRenderer_vulkan.h"

namespace df::vulkan
{

	vk::UniqueDescriptorSetLayout sDescriptorLayoutBuilder_vulkan::build( const vk::ShaderStageFlags _shader_stages )
	{
		DF_ProfilingScopeCpu;

		const cRenderer_vulkan* renderer = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );
		return build( renderer->getLogicalDevice(), _shader_stages );
	}

	vk::UniqueDescriptorSetLayout sDescriptorLayoutBuilder_vulkan::build( const vk::Device& _logical_device, const vk::ShaderStageFlags _shader_stages )
	{
		DF_ProfilingScopeCpu;

		for( vk::DescriptorSetLayoutBinding& binding: bindings )
			binding.stageFlags |= _shader_stages;

		const vk::DescriptorSetLayoutCreateInfo create_info( vk::DescriptorSetLayoutCreateFlags(), static_cast< uint32_t >( bindings.size() ), bindings.data() );

		return _logical_device.createDescriptorSetLayoutUnique( create_info ).value;
	}
}
