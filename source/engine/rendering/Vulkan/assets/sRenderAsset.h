#pragma once

#include "engine/misc/Misc.h"

namespace df::vulkan
{
	struct sRenderAsset
	{
		DF_DISABLE_COPY_AND_MOVE( sRenderAsset );

		explicit sRenderAsset();
		virtual ~sRenderAsset();

		unsigned vertex_array;
		unsigned vertex_buffer;
		unsigned element_buffer;
	};
}
