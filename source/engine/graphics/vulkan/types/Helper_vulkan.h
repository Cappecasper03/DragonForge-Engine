#pragma once

#include <string>
#include <vulkan/vulkan.hpp>

namespace df::vulkan::helper
{
	namespace util
	{
		vk::ShaderModule createShaderModule( const std::string& _name );
	}
}
