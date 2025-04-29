#include "sDescriptorWriter_vulkan.h"

#include "engine/profiling/ProfilingMacros.h"
#include "graphics/cRenderer.h"
#include "graphics/vulkan/cRenderer_vulkan.h"

namespace df::vulkan
{
	void sDescriptorWriter_vulkan::writeImage( const uint32_t _binding, const vk::ImageView& _image, const vk::ImageLayout _layout, const vk::DescriptorType _type )
	{
		DF_ProfilingScopeCpu;

		vk::DescriptorImageInfo& info = image_infos.emplace_back( nullptr, _image, _layout );

		vk::WriteDescriptorSet write;
		write.setDstBinding( _binding );
		write.setDescriptorCount( 1 );
		write.setDescriptorType( _type );
		write.setImageInfo( info );

		writes.push_back( write );
	}

	void sDescriptorWriter_vulkan::writeSampler( const uint32_t _binding, const vk::Sampler& _sampler, const vk::DescriptorType _type )
	{
		DF_ProfilingScopeCpu;

		vk::DescriptorImageInfo& info = image_infos.emplace_back( _sampler );

		vk::WriteDescriptorSet write;
		write.setDstBinding( _binding );
		write.setDescriptorCount( 1 );
		write.setDescriptorType( _type );
		write.setImageInfo( info );

		writes.push_back( write );
	}

	void sDescriptorWriter_vulkan::writeBuffer( const uint32_t _binding, const vk::Buffer& _buffer, const size_t _size, const size_t _offset, const vk::DescriptorType _type )
	{
		DF_ProfilingScopeCpu;

		vk::DescriptorBufferInfo& info = buffer_infos.emplace_back( _buffer, _offset, _size );

		vk::WriteDescriptorSet write;
		write.setDstBinding( _binding );
		write.setDescriptorCount( 1 );
		write.setDescriptorType( _type );
		write.setBufferInfo( info );

		writes.push_back( write );
	}
	void sDescriptorWriter_vulkan::clear()
	{
		DF_ProfilingScopeCpu;

		image_infos.clear();
		writes.clear();
		buffer_infos.clear();
	}

	void sDescriptorWriter_vulkan::updateSet( const vk::DescriptorSet _set )
	{
		DF_ProfilingScopeCpu;

		const cRenderer_vulkan* renderer = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );

		for( vk::WriteDescriptorSet& write: writes )
			write.setDstSet( _set );

		renderer->getLogicalDevice().updateDescriptorSets( writes, nullptr );
	}
}
