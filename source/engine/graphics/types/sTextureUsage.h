#pragma once

#include <vulkan/vulkan.hpp>

#include "sEnumBitmask.h"

namespace df
{
	struct sTextureUsage
	{
		sTextureUsage() = delete;

		enum eUsage
		{
			kTransferSource      = 1 << 0,
			kTransferDestination = 1 << 1,
			kStorage             = 1 << 2,
			kSampled             = 1 << 3,
			kColorAttachment     = 1 << 4,
		};

		using eUsageFlags = sEnumBitmask< eUsage >;

		static vk::ImageUsageFlags toVulkan( const eUsageFlags& _usage );
	};
}
