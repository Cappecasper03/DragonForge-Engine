#pragma once

#include <string>
#include <vulkan/vulkan.hpp>

#include "engine/misc/cColor.h"
#include "engine/misc/Misc.h"
#include "engine/rendering/assets/iModel.h"

struct aiMesh;
struct aiScene;
struct aiNode;

namespace df::vulkan
{
	class cMesh_vulkan;
	class cTexture_vulkan;

	class cModel_vulkan : public iModel
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( cModel_vulkan )

		friend cMesh_vulkan;

		struct sPushConstants
		{
			glm::mat4 world_matrix;
			cColor    color;
		};

		explicit cModel_vulkan( std::string _name );
		~cModel_vulkan() override = default;

		static iRenderCallback* createDefaultRenderCallback();

		vk::DescriptorSetLayout getTextureLayout() { return texture_layout.get(); }

	private:
		bool processNode( const aiNode* _node, const aiScene* _scene ) override;

		static vk::UniqueDescriptorSetLayout texture_layout;
	};
}
