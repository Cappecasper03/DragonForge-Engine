#pragma once

#include <glm/vec3.hpp>
#include <string>

#include "engine/misc/cColor.h"
#include "engine/rendering/assets/iQuad.h"
#include "sRenderAsset.h"

namespace df
{
	class cQuadManager;
}

namespace df::vulkan
{
	class cTexture;

	class cQuad : public sRenderAsset,
				  public iQuad
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( cQuad );

		friend cQuadManager;

		cQuad( std::string _name, const glm::vec3& _position, const glm::vec2& _size, const cColor& _color = color::white );
		~cQuad() override = default;

		void render() override;

	protected:
		struct sPushConstants
		{
			glm::mat4       u_world_matrix;
			glm::mat4       u_projection_view_matrix;
			VkDeviceAddress vertex_buffer;
		};
	};
}
