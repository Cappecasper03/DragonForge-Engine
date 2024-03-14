#include "sDesctriptorAllocator_vulkan.h"

#include <tracy/Tracy.hpp>

#include "engine/log/Log.h"

namespace df::vulkan
{
	void sDesctriptorAllocator_vulkan::create( const VkDevice _logical_device, const uint32_t _initial_sets, const std::span< sPoolSizeRatio >& _pool_ratios )
		: m_sets_per_pool( _initial_sets )
		, m_logical_device( _logical_device )
	{
		ZoneScoped;

		m_ratios.clear();

		for( const sPoolSizeRatio& ratio: _pool_ratios )
			m_ratios.push_back( ratio );

		const VkDescriptorPool pool = createPool( _initial_sets, _pool_ratios );
		m_sets_per_pool             = static_cast< uint32_t >( _initial_sets * 1.5f );

		m_ready_pools.push_back( pool );
	}

	void sDesctriptorAllocator_vulkan::destroy()
	{
		ZoneScoped;

		for( const VkDescriptorPool& pool: m_ready_pools )
			vkDestroyDescriptorPool( m_logical_device, pool, nullptr );

		m_ready_pools.clear();
		for( const VkDescriptorPool& pool: m_full_pools )
			vkDestroyDescriptorPool( m_logical_device, pool, nullptr );

		m_full_pools.clear();
	}

	void sDesctriptorAllocator_vulkan::clear()
	{
		ZoneScoped;

		for( const VkDescriptorPool& pool: m_ready_pools )
			vkResetDescriptorPool( m_logical_device, pool, 0 );

		for( const VkDescriptorPool& pool: m_full_pools )
		{
			vkResetDescriptorPool( m_logical_device, pool, 0 );
			m_ready_pools.push_back( pool );
		}

		m_full_pools.clear();
	}

	VkDescriptorSet sDesctriptorAllocator_vulkan::allocate( const VkDescriptorSetLayout _layout )
	{
		ZoneScoped;

		VkDescriptorPool pool = getPool();

		VkDescriptorSetAllocateInfo allocate_info{
			.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			.pNext              = nullptr,
			.descriptorPool     = pool,
			.descriptorSetCount = 1,
			.pSetLayouts        = &_layout,
		};

		VkDescriptorSet descriptor_set;
		VkResult        result = vkAllocateDescriptorSets( m_logical_device, &allocate_info, &descriptor_set );

		if( result == VK_ERROR_OUT_OF_POOL_MEMORY || result == VK_ERROR_FRAGMENTED_POOL )
		{
			m_full_pools.push_back( pool );
			pool                         = getPool();
			allocate_info.descriptorPool = pool;

			result = vkAllocateDescriptorSets( m_logical_device, &allocate_info, &descriptor_set );
			if( result == VK_ERROR_OUT_OF_POOL_MEMORY || result == VK_ERROR_FRAGMENTED_POOL )
				DF_LOG_ERROR( "Failed to allocate descriptor pool" );
		}

		return descriptor_set;
	}

	VkDescriptorPool sDesctriptorAllocator_vulkan::getPool()
	{
		ZoneScoped;

		VkDescriptorPool pool;

		if( !m_ready_pools.empty() )
		{
			pool = m_ready_pools.back();
			m_ready_pools.pop_back();
		}
		else
		{
			pool            = createPool( m_sets_per_pool, m_ratios );
			m_sets_per_pool = static_cast< uint32_t >( m_sets_per_pool * 1.5f );

			if( m_sets_per_pool > 4092 )
				m_sets_per_pool = 4092;
		}

		return pool;
	}

	VkDescriptorPool sDesctriptorAllocator_vulkan::createPool( const uint32_t _set_count, const std::span< sPoolSizeRatio >& _pool_ratios ) const
	{
		ZoneScoped;

		std::vector< VkDescriptorPoolSize > pool_sizes;
		for( const sPoolSizeRatio& ratio: _pool_ratios )
		{
			pool_sizes.push_back( VkDescriptorPoolSize{
				.type            = ratio.type,
				.descriptorCount = static_cast< uint32_t >( ratio.ratio * _set_count ),
			} );
		}

		const VkDescriptorPoolCreateInfo create_info{
			.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.flags         = 0,
			.maxSets       = _set_count,
			.poolSizeCount = static_cast< uint32_t >( pool_sizes.size() ),
			.pPoolSizes    = pool_sizes.data(),
		};

		VkDescriptorPool pool;
		vkCreateDescriptorPool( m_logical_device, &create_info, nullptr, &pool );
		return pool;
	}

	void sDescriptorWriter_vulkan::writeImage( const uint32_t _binding, const VkImageView _image, const VkSampler _sampler, const VkImageLayout _layout, const VkDescriptorType _type )
	{
		ZoneScoped;

		VkDescriptorImageInfo& info{
			.sampler     = _sampler,
			.imageView   = _image,
			.imageLayout = _layout,
		};

		const VkWriteDescriptorSet write{
			.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet          = nullptr,
			.dstBinding      = _binding,
			.descriptorCount = 1,
			.descriptorType  = _type,
			.pImageInfo      = &info,
		};

		writes.push_back( write );
	}

	void sDescriptorWriter_vulkan::writeBuffer( const uint32_t _binding, const VkBuffer _buffer, const size_t _size, const size_t _offset, const VkDescriptorType _type )
	{
		ZoneScoped;

		VkDescriptorBufferInfo& info = buffer_infos.emplace_back( VkDescriptorBufferInfo{
			.buffer = _buffer,
			.offset = _offset,
			.range  = _size,
		} );

		const VkWriteDescriptorSet write{
			.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet          = nullptr,
			.dstBinding      = _binding,
			.descriptorCount = 1,
			.descriptorType  = _type,
			.pBufferInfo     = &info,
		};

		writes.push_back( write );
	}
	void sDescriptorWriter_vulkan::clear()
	{
		ZoneScoped;

		image_infos.clear();
		writes.clear();
		buffer_infos.clear();
	}

	void sDescriptorWriter_vulkan::updateSet( const VkDevice _logical_device, const VkDescriptorSet _set )
	{
		for( VkWriteDescriptorSet& write: writes )
			write.dstSet = _set;

		vkUpdateDescriptorSets( _logical_device, static_cast< uint32_t >( writes.size() ), writes.data(), 0, nullptr );
	}
}
