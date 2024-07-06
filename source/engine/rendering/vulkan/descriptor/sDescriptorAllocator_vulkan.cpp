#include "sDescriptorAllocator_vulkan.h"

#include <tracy/Tracy.hpp>

#include "engine/log/Log.h"

namespace df::vulkan
{
	sDescriptorAllocator_vulkan::sDescriptorAllocator_vulkan()
		: m_sets_per_pool( 0 )
	{
		ZoneScoped;
	}

	void sDescriptorAllocator_vulkan::create( const vk::Device _logical_device, const uint32_t _initial_sets, const std::span< sPoolSizeRatio >& _pool_ratios )
	{
		ZoneScoped;

		m_sets_per_pool  = _initial_sets;
		m_logical_device = _logical_device;
		m_ratios.clear();

		for( const sPoolSizeRatio& ratio: _pool_ratios )
			m_ratios.push_back( ratio );

		const vk::DescriptorPool pool = createPool( _initial_sets, _pool_ratios );
		m_sets_per_pool               = static_cast< uint32_t >( static_cast< float >( _initial_sets ) * 1.5f );

		m_ready_pools.push_back( pool );
	}

	void sDescriptorAllocator_vulkan::destroy()
	{
		ZoneScoped;

		m_ready_pools.clear();
		m_full_pools.clear();

		for( vk::UniqueDescriptorPool& pool: m_pools )
			pool.release();

		m_pools.clear();
	}

	void sDescriptorAllocator_vulkan::clear()
	{
		ZoneScoped;

		m_ready_pools.clear();
		m_full_pools.clear();

		for( vk::UniqueDescriptorPool& pool: m_pools )
		{
			m_logical_device.resetDescriptorPool( pool.get() );
			m_ready_pools.push_back( pool.get() );
		}
	}

	vk::DescriptorSet sDescriptorAllocator_vulkan::allocate( const vk::DescriptorSetLayout _layout )
	{
		ZoneScoped;

		vk::DescriptorPool pool = getPool();

		vk::DescriptorSetAllocateInfo allocate_info( pool, {}, _layout );

		std::vector< vk::DescriptorSet > descriptor_sets = m_logical_device.allocateDescriptorSets( allocate_info ).value;

		if( descriptor_sets.empty() )
		{
			m_full_pools.push_back( pool );
			pool                         = getPool();
			allocate_info.descriptorPool = pool;

			try
			{
				descriptor_sets = m_logical_device.allocateDescriptorSets( allocate_info ).value;
			}
			catch( vk::Result )
			{
				if( descriptor_sets.empty() )
					DF_LOG_ERROR( "Failed to allocate descriptor pool" );
			}
		}

		return descriptor_sets.front();
	}

	vk::DescriptorPool sDescriptorAllocator_vulkan::getPool()
	{
		ZoneScoped;

		vk::DescriptorPool pool;

		if( !m_ready_pools.empty() )
		{
			pool = m_ready_pools.back();
			m_ready_pools.pop_back();
			m_ready_pools.push_back( pool );
		}
		else
		{
			pool = createPool( m_sets_per_pool, m_ratios );
			m_ready_pools.push_back( pool );
			m_sets_per_pool = static_cast< uint32_t >( static_cast< float >( m_sets_per_pool ) * 1.5f );

			if( m_sets_per_pool > 4092 )
				m_sets_per_pool = 4092;
		}

		return pool;
	}

	vk::DescriptorPool sDescriptorAllocator_vulkan::createPool( const uint32_t _set_count, const std::span< sPoolSizeRatio >& _pool_ratios ) const
	{
		ZoneScoped;

		std::vector< vk::DescriptorPoolSize > pool_sizes;

		for( const sPoolSizeRatio& ratio: _pool_ratios )
			pool_sizes.emplace_back( ratio.type, static_cast< uint32_t >( ratio.ratio * static_cast< float >( _set_count ) ) );

		const vk::DescriptorPoolCreateInfo create_info( {}, _set_count, static_cast< uint32_t >( pool_sizes.size() ), pool_sizes.data() );

		return m_logical_device.createDescriptorPool( create_info ).value;
	}
}
