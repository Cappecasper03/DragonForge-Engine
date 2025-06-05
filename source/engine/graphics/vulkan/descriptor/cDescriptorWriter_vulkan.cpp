#include "cDescriptorWriter_vulkan.h"

#include "engine/graphics/cRenderer.h"
#include "engine/graphics/vulkan/assets/textures/cSampler_vulkan.h"
#include "engine/graphics/vulkan/cGraphicsDevice_vulkan.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::vulkan
{
	void cDescriptorWriter_vulkan::writeImage( const uint32_t _binding, const vk::ImageView& _image, const vk::ImageLayout _layout, const vk::DescriptorType _type )
	{
		DF_ProfilingScopeCpu;

		vk::DescriptorImageInfo& info = m_image_infos.emplace_back( nullptr, _image, _layout );

		vk::WriteDescriptorSet write;
		write.setDstBinding( _binding );
		write.setDescriptorCount( 1 );
		write.setDescriptorType( _type );
		write.setImageInfo( info );

		m_writes.push_back( write );
	}

	void cDescriptorWriter_vulkan::writeSampler( const uint32_t _binding, const iSampler* _sampler, const vk::DescriptorType _type )
	{
		DF_ProfilingScopeCpu;

		vk::DescriptorImageInfo& info = m_image_infos.emplace_back( reinterpret_cast< const cSampler_vulkan* >( _sampler )->get() );

		vk::WriteDescriptorSet write;
		write.setDstBinding( _binding );
		write.setDescriptorCount( 1 );
		write.setDescriptorType( _type );
		write.setImageInfo( info );

		m_writes.push_back( write );
	}

	void cDescriptorWriter_vulkan::writeBuffer( const uint32_t _binding, const vk::Buffer& _buffer, const size_t _size, const size_t _offset, const vk::DescriptorType _type )
	{
		DF_ProfilingScopeCpu;

		vk::DescriptorBufferInfo& info = m_buffer_infos.emplace_back( _buffer, _offset, _size );

		vk::WriteDescriptorSet write;
		write.setDstBinding( _binding );
		write.setDescriptorCount( 1 );
		write.setDescriptorType( _type );
		write.setBufferInfo( info );

		m_writes.push_back( write );
	}
	void cDescriptorWriter_vulkan::clear()
	{
		DF_ProfilingScopeCpu;

		m_image_infos.clear();
		m_writes.clear();
		m_buffer_infos.clear();
	}

	void cDescriptorWriter_vulkan::updateSet( const vk::DescriptorSet _set )
	{
		DF_ProfilingScopeCpu;

		const cGraphicsDevice_vulkan* renderer = reinterpret_cast< cGraphicsDevice_vulkan* >( cRenderer::getGraphicsDevice() );

		for( vk::WriteDescriptorSet& write: m_writes )
			write.setDstSet( _set );

		renderer->getLogicalDevice().updateDescriptorSets( m_writes, nullptr );
	}
}
