#include "sTextureUsage.h"

#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	vk::ImageUsageFlags sTextureUsage::toVulkan( const eUsageFlags& _usage )
	{
		DF_ProfilingScopeCpu;

		switch( _usage.value )
		{
			case kTransferSource:      return vk::ImageUsageFlagBits::eTransferSrc;
			case kTransferDestination: return vk::ImageUsageFlagBits::eTransferDst;

			case kStorage: return vk::ImageUsageFlagBits::eStorage;
			case kSampled: return vk::ImageUsageFlagBits::eSampled;

			case kColorAttachment: return vk::ImageUsageFlagBits::eColorAttachment;
		}

		vk::ImageUsageFlags format{};

		if( _usage & kTransferSource )
			format |= vk::ImageUsageFlagBits::eTransferSrc;
		if( _usage & kTransferDestination )
			format |= vk::ImageUsageFlagBits::eTransferDst;

		if( _usage & kStorage )
			format |= vk::ImageUsageFlagBits::eStorage;
		if( _usage & kSampled )
			format |= vk::ImageUsageFlagBits::eSampled;

		if( _usage & kColorAttachment )
			format |= vk::ImageUsageFlagBits::eColorAttachment;

		return format;
	}
}
