#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <string>

#include "engine/misc/cColor.h"
#include "engine/rendering/assets/iQuad.h"
#include "sRenderAsset_vulkan.h"

namespace df
{
	struct iRenderCallback;

	class cQuadManager;
}

namespace df::vulkan
{
	class cTexture_vulkan;

	class cQuad_vulkan : public sRenderAsset_vulkan,
						 public iQuad
	{
	public:
		DF_DisableCopyAndMove( cQuad_vulkan );

		struct sPushConstants
		{
			glm::mat4 world_matrix;
		};

		cQuad_vulkan( std::string _name, const glm::vec3& _position, const glm::vec2& _size, const cColor& _color = color::white );
		~cQuad_vulkan() override = default;

		bool loadTexture( const std::string& _file_path, bool _mipmapped, int _mipmaps, bool _flip_vertically_on_load ) override;

		void render() override;

		static iRenderCallback* createDefaults();
		static void             destroyDefaults();

		static vk::DescriptorSetLayout getLayout() { return s_quad_layout.get(); }

	private:
		static iRenderCallback* createDefaultsDeferred();

		static vk::UniqueDescriptorSetLayout s_quad_layout;
	};
}
