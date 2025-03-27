#pragma once

#include "engine/rendering/assets/iMesh.h"
#include "sRenderAsset_vulkan.h"

struct aiScene;
struct aiMesh;

namespace df::vulkan
{
	class cTexture_vulkan;
	class cModel_vulkan;

	class cMesh_vulkan : public sRenderAsset_vulkan,
						 public iMesh
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( cMesh_vulkan )

		friend cModel_vulkan;

		struct sPushConstants
		{
			glm::mat4 world_matrix;
		};

		explicit cMesh_vulkan( const aiMesh* _mesh, const aiScene* _scene, cModel_vulkan* _parent );

		void render() override;

		static vk::DescriptorSetLayout getLayout() { return s_mesh_layout.get(); }

	private:
		void createTextures( const aiMesh* _mesh, const aiScene* _scene ) override;

		static vk::UniqueDescriptorSetLayout s_mesh_layout;
	};
}
