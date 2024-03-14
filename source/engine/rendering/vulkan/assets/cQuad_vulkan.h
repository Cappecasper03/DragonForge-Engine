#pragma once

#include <glm/vec3.hpp>
#include <string>

#include "engine/misc/cColor.h"
#include "engine/rendering/assets/iQuad.h"
#include "sRenderAsset_vulkan.h"

namespace df
{
	class cQuadManager;
}

namespace df::vulkan
{
	class cTexture_vulkan;

	class cQuad_vulkan : public sRenderAsset_vulkan,
						 public iQuad
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( cQuad_vulkan );

		struct sVertexConstants
		{
			glm::mat4       u_world_matrix;
			VkDeviceAddress vertex_buffer;
		};

		cQuad_vulkan( std::string _name, const glm::vec3& _position, const glm::vec2& _size, const cColor& _color = color::white );
		~cQuad_vulkan() override = default;

		void render() override;
	};
}
