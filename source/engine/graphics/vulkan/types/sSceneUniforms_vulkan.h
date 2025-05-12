#pragma once

#include "engine/core/math/cMatrix.h"

namespace df::vulkan
{
	struct sVertexSceneUniforms_vulkan
	{
		cMatrix4f view_projection;
	};
}
