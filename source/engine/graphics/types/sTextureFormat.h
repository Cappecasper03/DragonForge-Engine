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

			kRGB16f,
		};

		static int        toOpenGl( eFormat _format );
		static vk::Format toVulkan( eFormat _format );
	};
}
