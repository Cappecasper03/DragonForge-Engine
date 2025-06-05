#pragma once

#include <vulkan/vulkan.hpp>

namespace df
{
	struct sTextureFormat
	{
		sTextureFormat() = delete;

		enum eFormat
		{
			kRed,
			kRGB,
			kRGBA,

			kRGB16sf,
		};

		static int        toOpenGlInternal( eFormat _format );
		static int        toOpenGlBase( eFormat _format );
		static vk::Format toVulkan( eFormat _format );
	};
}
