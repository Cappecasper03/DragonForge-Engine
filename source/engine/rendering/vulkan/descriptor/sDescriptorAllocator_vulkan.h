#pragma once

#include <span>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace df::vulkan
{

	struct sDescriptorAllocator_vulkan
	{
		struct sPoolSizeRatio
		{
			VkDescriptorType type;
			float            ratio;
		};

		void create( VkDevice _logical_device, uint32_t _initial_sets, const std::span< sPoolSizeRatio >& _pool_ratios );
		void destroy();
		void clear();

		VkDescriptorSet allocate( VkDescriptorSetLayout _layout );

	protected:
		VkDescriptorPool getPool();
		VkDescriptorPool createPool( uint32_t _set_count, const std::span< sPoolSizeRatio >& _pool_ratios ) const;

		std::vector< sPoolSizeRatio >   m_ratios;
		std::vector< VkDescriptorPool > m_full_pools;
		std::vector< VkDescriptorPool > m_ready_pools;
		uint32_t                        m_sets_per_pool = 0;

	private:
		VkDevice m_logical_device = nullptr;
	};
}
