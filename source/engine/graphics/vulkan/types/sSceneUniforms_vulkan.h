#pragma once

#include "engine/core/math/cMatrix.h"
#include "engine/graphics/lights/sLight.h"

namespace df::vulkan
{
	struct sVertexSceneUniforms_vulkan
	{
		cMatrix4f view_projection;
	};

	struct sFragmentSceneUniforms_vulkan
	{
		sLight   lights[ 64 ];
		unsigned light_count = 0;
	};
}
