#pragma once

#include <string>
#include <vulkan/vulkan.hpp>

namespace df::vulkan::helper
{
	namespace util
	{
		void transitionImage( const vk::CommandBuffer& _command_buffer, const vk::Image& _image, vk::ImageLayout _current_layout, vk::ImageLayout _new_layout );

		void copyImageToImage( const vk::CommandBuffer& _command_buffer,
		                       const vk::Image&         _source,
		                       const vk::Image&         _destination,
		                       vk::Extent2D             _source_size,
		                       vk::Extent2D             _destination_size );

		vk::ShaderModule createShaderModule( const std::string& _name );
	}
}
