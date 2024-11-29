#pragma once

#include <glm/ext/matrix_float4x4.hpp>

namespace df::vulkan
{
	struct sVertexSceneUniforms_vulkan
	{
		glm::mat4 view_projection;
	};
}
