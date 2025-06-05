#pragma once

#include <vulkan/vulkan.hpp>

namespace df
{
	struct sSamplerParameter
	{
		sSamplerParameter() = delete;

		enum eName
		{
			kMinFilter,
			kMagFilter,
			kMipMapFilter,

			kWrapS,
			kWrapT,
			kWrapR,
		};

		enum eParameter
		{
			kNearest,
			kLinear,

			kNearestMipmapNearest,
			kLinearMipmapNearest,
			kNearestMipmapLinear,
			kLinearMipmapLinear,

			kClampToEdge,
			kClampToBorder,
			kMirroredRepeat,
			kRepeat,
			kMirrorClampToEdge
		};

		static int toOpenGl( eName _name );
		static int toOpenGl( eParameter _parameter );

		static int toVulkan( eParameter _parameter );
	};
}
