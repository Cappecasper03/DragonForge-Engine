#pragma once

#include <span>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

namespace df::vulkan
{

	struct sDescriptorAllocator_vulkan
	{
		struct sPoolSizeRatio
		{
			vk::DescriptorType type;
			float              ratio;
		};

		sDescriptorAllocator_vulkan();

		void create( vk::Device _logical_device, uint32_t _initial_sets, const std::span< sPoolSizeRatio >& _pool_ratios );
		void destroy();
		void clear();

		vk::DescriptorSet allocate( vk::DescriptorSetLayout _layout );

	protected:
		vk::DescriptorPool getPool();
		vk::DescriptorPool createPool( uint32_t _set_count, const std::span< sPoolSizeRatio >& _pool_ratios ) const;

		std::vector< sPoolSizeRatio >     m_ratios;
		std::vector< vk::DescriptorPool > m_full_pools;
		std::vector< vk::DescriptorPool > m_ready_pools;
		uint32_t                          m_sets_per_pool;

	private:
		std::vector< vk::UniqueDescriptorPool > m_pools;

		vk::Device m_logical_device = nullptr;
	};
}
