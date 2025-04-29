#pragma once

#include <glm/ext/matrix_float4x4.hpp>

namespace df::vulkan
{
	struct sVertexSceneUniforms_vulkan
	{
		cMatrix4f view_projection;
	};
}
